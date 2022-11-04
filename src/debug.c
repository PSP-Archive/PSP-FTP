#include <pspkernel.h>
#include <pspdebug.h>
#include <string.h>
#include <pspctrl.h>
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"
#include "ftpclient.h"

// Debug commands
int debugCurrCMD = 0;	// Currently selected debug command
//							   0		   1		        2	        3		4	              5		     6		        7	           8                 9                  10                  11		   12
char* debugCMD[CMDSIZE] = {	"LIST", "CWD", "RETR", "STOR", "PWD", "MKDIR", "RMDIR", "CHMOD", "DEL", "SYST", "PASV", "RECON", "QUIT" };

// clears a full line on the screen
void debugClearLn(int num) {
	pspDebugScreenSetXY(0, num);
	//clears line with debug font
	pspDebugScreenPrintf("                                                                    ");
	pspDebugScreenSetXY(0, num);
}

// prints the title at the top of the screen
void debugTitle(char* info) {
	debugClearLn(0);
	pspDebugScreenSetTextColor(DEBUG_COLOR1);
	if ( info == NULL || info == "" || info == " " ) {
		pspDebugScreenPrintf("[ FTP Client .99a]");	
	} else {
		pspDebugScreenPrintf("[ FTP Client .99a] [%s]", info);	
	}
	pspDebugScreenSetTextColor(WHITE);
}

// prints commands at the bottom of the screen
void debugFooter(void) {
	
	if ( ftpStatus() >= STATUS_LOGGEDIN ) {
		if ( ftpStatus() == STATUS_TRANSFERRING ) {
			// transferring footer with status
			pspDebugScreenSetTextColor(DEBUG_COLOR2);
			// TODO: provide option to abort transfer
			debugClearLn(31);
			
				
			if ( ftpCurrentRETR() >= ( 1024 * 1024 ) || ftpCurrentSTOR() >= (1024 * 1024) ) { 
				float dl = ((float)(ftpCurrentRETR())) / (1024*1024);
				float ul = ((float)(ftpCurrentSTOR())) / (1024*1024);
				pspDebugScreenPrintf("D/L: %.2fmb | U/L: %.2fmb", dl, ul);
			} else { 
				float dl = ((float)(ftpCurrentRETR())) / 1024;
				float ul = ((float)(ftpCurrentSTOR())) / 1024;
				pspDebugScreenPrintf("D/L: %.0fkb | U/L: %.0fkb", dl, ul);
			}
			
			debugClearLn(32);
			pspDebugScreenPrintf("Abort Transfer not yet supported");
			
		} else {
			// standard footer with command options
			debugClearLn(32);
			debugClearLn(31);
			int i = 0;
			while ( i <= CMDSIZE - 1) {
				/*
				if ( i > 9 ) {
					debugClearLn(33);
				} */
				if ( i == debugCurrCMD ) {
					pspDebugScreenSetTextColor(DEBUG_COLOR2);
					pspDebugScreenPrintf("[%s] ", debugCMD[i]);
				} else {
					pspDebugScreenSetTextColor(DEBUG_COLOR1);
					pspDebugScreenPrintf("%s ", debugCMD[i]);
				}
				i++;
			}
		}
	} 
	
	/*
	else {
		// no footer
		debugClearLn(32);
		debugClearLn(31);
	}
	*/

	// reset text color
	pspDebugScreenSetTextColor(WHITE);

}

// returns the selected command for debugging
int debugCurrent(void) {
	return debugCurrCMD;
}

// increases the selected command for debugging
void debugCMDInc(void) {
	if ( debugCurrCMD < CMDSIZE - 1 ) {
		debugCurrCMD++;
	} else {
		debugCurrCMD = 0;
	}
	debugFooter();
}

// decreases the selected command for debugging
void debugCMDDec(void) {
	if ( debugCurrCMD > 0 ) {
		debugCurrCMD--;
	} else {
		debugCurrCMD = CMDSIZE - 1;
	}
	debugFooter();
}

// Controls the location of where lines are printed to on the screen
// Print on all lines except top and bottom, which are 
// saved for input/info

void debugLine(char *fmt, ...) {
	va_list opt;
	va_start(opt, fmt);
	char buff[MAXLENGTH];
	// format the string
	vsnprintf(buff, (size_t) sizeof(buff), fmt, opt);
	
	static char printBuff[MAXLINES+1][MAXLENGTH+1];
	static int currLine = STARTLINE - 1;
	
	//pspDebugScreenSetBackColor(BLACK);
	//pspDebugScreenSetTextColor(GREEN);
	
	// Add the additional line to the buffer
	strcpy(printBuff[currLine - STARTLINE], buff);
			
	// Check to see if we have reached the maximum # of lines to show on the screen
	if ( currLine >= MAXLINES + STARTLINE - 1) {
		currLine = MAXLINES + STARTLINE - 1;
		
		// Reorder the lines, removing the oldest (0)
		int i = 1;
		while( i <= MAXLINES - 1) {
			//redraw old lines at the new location
			strcpy(printBuff[i - 1], printBuff[i]);

			//Clear old line on screen
			debugClearLn(STARTLINE + i - 2);
			
			// Print the line to the screen at the new location
			pspDebugScreenPrintf("%.2d: %s", i, printBuff[i - 1]);
			i++;
		}
	} else {
		// Print the new line to the screen
		debugClearLn(currLine);
		pspDebugScreenPrintf("%.2d: %s", currLine - 1, buff);
		currLine++;
	}

	
}

int debugInput(char* input, char* desc, char* defaultval, int charset) {
	
	int tempstrlen;							// current length of the inputted string
	if ( defaultval != NULL ) {
		strcpy(input, defaultval);
		tempstrlen = strlen(defaultval);
		input[tempstrlen] = '\0';
	} else {
		tempstrlen = 0;
		//input[0] = '\0';
	}
	
	struct charset keys[5];
	
	strcpy(keys[0].name,"All\0");
	strcpy(keys[0].chars,"abcdefghijklmnopqrstuvwxyz0123456789:,\[](){}!@#$%^&*;|+=<>_-/. \0");
	keys[0].selected = 0;
	strcpy(keys[1].name,"Letters\0");
	strcpy(keys[1].chars,"abcdefghijklmnopqrstuvwxyz\0");
	keys[1].selected = 0;
	strcpy(keys[2].name,"Special\0");
	strcpy(keys[2].chars,":,\[](){}!@#$%^&*;|+=<>_-/. \0");
	keys[2].selected = 0;
	strcpy(keys[3].name,"Numbers\0");
	strcpy(keys[3].chars,"0123456789\0");
	keys[3].selected = 0;
	strcpy(keys[4].name,"IP Addr\0");
	strcpy(keys[4].chars,"0123456789.\0");
	keys[4].selected = 0;
	
	
	// initialize controls
	SceCtrlData pad;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	// bPressed starts as pressed to delay input so we don't pick up an accidental keypress
	int bPressed = 1;
	int waitforinput = 0;
	while ( waitforinput == 0 && ftpExited() == 0) {
		// delay 200ms for key input if a button was pressed
		if ( bPressed == 1) {
			sceKernelDelayThread(200 * 1000);
			bPressed = 0;

			debugClearLn(INPUT_LINE2);
			pspDebugScreenSetTextColor(DEBUG_COLOR1);	// set color
			pspDebugScreenPrintf("U/D/L/R change input | [] char set (%s) | X confirm | O cancel", keys[charset].name);

			debugClearLn(INPUT_LINE1);
			pspDebugScreenSetTextColor(DEBUG_COLOR2);
			if ( tempstrlen == 0 ) {
				pspDebugScreenPrintf("%s: %c", desc, keys[charset].chars[keys[charset].selected]);
			} else {
				pspDebugScreenPrintf("%s: '%s'%c", desc, input, keys[charset].chars[keys[charset].selected]);
			}

		}
		sceCtrlReadBufferPositive(&pad, 1); 
		if (pad.Buttons & PSP_CTRL_UP){
			// move up one character
			if ( keys[charset].selected == (strlen(keys[charset].chars) - 1) ) {
				keys[charset].selected = 0;
			} else {
				keys[charset].selected++;
			}
			bPressed = 1;
			
		}
		if (pad.Buttons & PSP_CTRL_DOWN){
			// move down one character
			if ( keys[charset].selected == 0 ) {
				keys[charset].selected = strlen(keys[charset].chars) - 1;
			} else {
				keys[charset].selected--;
			}
			bPressed = 1;

		}
		if (pad.Buttons & PSP_CTRL_RIGHT){
			// move forward to next character, if we haven't reached the buffer size
			//if ( tempstrlen < strlen(input) ) {
				input[tempstrlen++]=keys[charset].chars[keys[charset].selected];
				input[tempstrlen] = '\0';				
				
				bPressed = 1;
				keys[charset].selected = 0;
			//}
		}
		if (pad.Buttons & PSP_CTRL_LEFT){
			// move backward to previous character
			tempstrlen--;
			input[tempstrlen]='\0';
			bPressed = 1;
			keys[charset].selected = 0;

		}
		if (pad.Buttons & PSP_CTRL_CROSS) {
			// success
			
			// restore original color
			pspDebugScreenSetTextColor(WHITE);
			
			// clear bottom 2 lines
			debugClearLn(INPUT_LINE2);
			debugClearLn(INPUT_LINE1);
			waitforinput = 1;
			bPressed = 1;
			
			// redraw footer
			debugFooter();
			
			return 1;
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
			// user cancelled
			strcpy(input, "\0");
			
			// restore original color
			pspDebugScreenSetTextColor(WHITE);
			
			// clear bottom 2 lines
			debugClearLn(INPUT_LINE2);
			debugClearLn(INPUT_LINE1);
			waitforinput = 1;
			
			// redraw footer
			debugFooter();
			
			return 0;
		}
		if (pad.Buttons & PSP_CTRL_SQUARE) {
			// change char set being used
			
			bPressed = 1;
			
			if (charset < 4) {
				charset++;
			} else {
				charset = 0;
			}
	
		}
	}
	// must have exited
	return 0;	
}
