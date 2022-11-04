#include <pspkernel.h>
#include <pspdebug.h>
#include <string.h>
#include <pspiofilemgr.h>
#include <pspiofilemgr_fcntl.h>
#include <stdio.h>
#include <pspctrl.h>

#include "picker.h"
#include "ftpclient.h"
#include "nlh.h"
// very simple UI for picking from a short list


void my_initpicker(PICKER* pickerP, const char* szTitle)
{
    strncpy(pickerP->szTitle, szTitle, MAX_PICK_TITLE);
    pickerP->szTitle[MAX_PICK_TITLE] = '\0';
    pickerP->pick_count = 0;
    pickerP->pick_start = -1;
}

int my_addpick(PICKER* pickerP, const char* szBig, const char* szFinePrint, u32 userData)
{
    PICKER_INFO* pickP;
    if (pickerP->pick_count >= MAX_PICK)
        return 0; // no room
    pickP = &(pickerP->picks[pickerP->pick_count++]);

    strncpy(pickP->szBig, szBig, MAX_PICK_MAINSTR);
    pickP->szBig[MAX_PICK_MAINSTR] = '\0';
    if (szFinePrint == NULL)
        szFinePrint = "";
    strncpy(pickP->szFinePrint, szFinePrint, MAX_PICK_FINEPRINT);
    pickP->szFinePrint[MAX_PICK_FINEPRINT] = '\0';
    pickP->userData = userData;
    return 1;
}

int my_loadpicks_fromfile(PICKER* pickerP, const char* szFile, int bParseIP)
    // returns number of entries read
{
    FILE* pf = fopen(szFile, "rt");
    if (pf == 0)
        return 0;
    char szLine[128];
    int nAdded = 0;
    while (fgets(szLine, sizeof(szLine), pf) != NULL)
    {
        // CR-LF will cause blank lines
        if (szLine[0] == '\0' || szLine[0] == ';')
            continue;   // skip

        // format is "BIG_STRING;FinePrint"
        char* szFinePrint = strchr(szLine, ';');
        if (szFinePrint != NULL)
            *szFinePrint++ = '\0';
        u32 userData = 0;
        if (bParseIP)
        {
            // parse szLine for ?.?.?.? IP address
			userData = sceNetInetInetAddr(szLine);
            if (userData == 0)
                continue;   // bad, skip it
        }
		// only first 5 will fit
		if (my_addpick(pickerP, szLine, szFinePrint, userData))
            nAdded++;
    }
    fclose(pf);
    return nAdded;
}

/////////////////////////////////////////////////////
// Actual UI


// pixel coordinates (use pgPrint4AtPixel)
#define Y_TITLE 0
#define X_TITLE 0

#define Y_PICK(i)  (32+8+48*(i))
#define X_PICK (2*8*4)
#define STR_PICK "=>"
#define Y_PICK_FINEPRINT(i) (Y_PICK(i)+32) // after big font
#define X_PICK_FINEPRINT (120)

int my_picker(const PICKER* pickerP)
{
    int iPick = pickerP->pick_start;
    u32 buttonsLast = 0;
    int bRedraw = 1;

    if (pickerP->pick_count == 0)
        return -1; // nothing to pick

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); // digital

    while (1)
    {
        int i;
		SceCtrlData pad;
        u32 buttonsNew;

	    if (pickerP->pick_count == 1)
        {
            // auto-pick if only one
            iPick = 0;
            break;
        } else {
			// set first connection as default
			iPick = 0;
		}

        if (bRedraw)
        {
			pspDebugScreenClear();
			pspDebugScreenPrintf("%s\n", pickerP->szTitle);
			for (i = 0; i < pickerP->pick_count; i++)
            {
			    PICKER_INFO const* pickP = &pickerP->picks[i];
                if (i == iPick)
                {
					// currently selected
					pspDebugScreenPrintf("-> %s (%s)\n", pickP->szBig, pickP->szFinePrint);
                } else {
					// other selection
					pspDebugScreenPrintf("%s (%s)\n", pickP->szBig, pickP->szFinePrint);
				}
            }
            bRedraw = 0;
        }

		if ( ftpExited() == 1 ) {
			break;
		}
		
        // handle up/down inputs
		sceCtrlReadBufferPositive(&pad, 1); 
        buttonsNew = pad.Buttons & ~buttonsLast;
        buttonsLast = pad.Buttons;
        if (buttonsNew & PSP_CTRL_DOWN)
        {
            iPick++;
            if (iPick >= pickerP->pick_count)
                iPick = pickerP->pick_count-1;
            bRedraw = 1;
        }
        else if (buttonsNew & PSP_CTRL_UP)
        {
            iPick--;
            if (iPick < 0)
                iPick = 0;
            bRedraw = 1;
        }
        else if (buttonsNew & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS))
        {
            if (iPick >= 0 && iPick < pickerP->pick_count)
                break; // picked !
        }
        else if (buttonsNew & PSP_CTRL_TRIANGLE)
        {
            iPick = -1; // cancel
            break;
        }
    }
	pspDebugScreenClear();
			
    return iPick;
}
