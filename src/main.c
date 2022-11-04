#include <pspkernel.h>
#include <string.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <stdlib.h>

#include "debug.h"
#include "ftpclient.h"
#include "shot.c"

#include "nlh.h"


//NOTE: kernel mode module flag and kernel mode thread are both required
PSP_MODULE_INFO("PSP-FTP", 0x1000, 1, 1); /* 0x1000 REQUIRED!!! */
PSP_MAIN_THREAD_ATTR(0); /* 0 REQUIRED!!! */
PSP_MAIN_THREAD_STACK_SIZE_KB(32); /* smaller stack for kernel thread */


int done = 0;

// server info
char serverip[25];
char serverport[5];
char username[25];
char password[25];

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	quitFTP();
	done = 1;
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;
    
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

// get server info from user
int GetInfo(void) {
	if ( DEBUG_USE_STATIC == 0 ) {
		// Input server information
		debugInput(serverip, "Server IP", "", CHAR_IP);
		if ( strlen(serverip) != 0 ) {
			debugInput(serverport, "Server Port", "21", CHAR_NUMBERS);
			if ( strlen(serverport) != 0 ) {
				debugInput(username, "Server Username (leave blank for anon)", "", CHAR_ALL);
				if ( strlen(username) != 0 ) {
					debugInput(password, "Server Password", "", CHAR_ALL);
					if ( strlen(password) != 0 ) {
						return 0;
					}
				} else {
					// log on anonymously
					strcpy(username, "anonymous");
					strcpy(password, "pspuser@anonymous.com");
					return 0;
				}
			}
		}
		// something missing
		return -1;
	} else {
		// **** temp static info for testing ****
		strcpy(serverip, "***.***.*.***");
		strcpy(serverport, "**");
		strcpy(username, "***");
		strcpy(password, "***");
		return 0;
	}
}

// attempt to connect to server
int AttemptConnect(void) {
	int connected = ftpConnect(serverip, atoi(serverport));
	if ( connected == 1 ) {
		// login
		int loggedin = ftpLogin(username, password);
		if ( loggedin == 1 ) {
			// do some stuff on connect
			ftpSYST();
			ftpPWD();
		}
		return 0;
	} else {
		return -1;
	}
}

int 
ftp_user_thread(void)
{
	SceCtrlData pad;
	int err;
	
	startFTP(&module_info);
	
	// get server info from user
	err = GetInfo();
	if ( err != 0 ) {
		// cancelled login info needed to run
		done = 1;
	}
	
	// attempt to connect to server with retry
	//int retries = 3;
	//int retrycount = 0;
	//while ( retrycount < retries ) {
		err = AttemptConnect();
		//if ( err != 0 ) {
		//	debugLine("Unable to Connect (#%d), retrying ...", retrycount);
		//	retrycount++;
		//	if ( retrycount == retries ) {
				// unable to connect after specified number of retries
		//		done = 1;
		//		break;
		//	}
		//} else {
			// connected successfully
		//	break;
		//}
	//}
	
	int bPressed = 0;
	while ( done == 0 ) {
		
		sceCtrlReadBufferPositive(&pad, 1); 
		if (pad.Buttons != 0){
			if (pad.Buttons & PSP_CTRL_LTRIGGER){
				debugCMDDec();
				bPressed = 1;
			} 
			if (pad.Buttons & PSP_CTRL_RTRIGGER){
				debugCMDInc();
				bPressed = 1;
			}
			if (pad.Buttons & PSP_CTRL_SQUARE) {
				if ( ftpStatus() == STATUS_TRANSFERRING ) {
					// if transfer is in progress, send abort command
					ftpABOR();
				}
			}
			if (pad.Buttons & PSP_CTRL_SELECT){
				createscreenshot("ms0:/", "ftpc.tga");
				bPressed = 1;
			}
			if (pad.Buttons & PSP_CTRL_CROSS){
				if (debugCurrent() == 0) {
					// LIST
					remoteDirent *mylist;
					mylist = ftpLIST();
					
					if ( mylist != NULL ) {
						// show dirs
						int i;
						for (i = 0; i < mylist->totalCount; i++) {
							if (FIO_SO_ISDIR(mylist->files[i].st_attr)) {
								debugLine("* %s", mylist->files[i].d_name);
							}
						}
			
						// show files
						for (i = 0; i < mylist->totalCount; i++) {
							if (FIO_SO_ISREG(mylist->files[i].st_attr)) {
								if ( atof(mylist->files[i].st_size) >= ( 1024 * 1024 ) ) { 
									float fsize = atof(mylist->files[i].st_size) / (1024*1024);
									debugLine("%s (%.2fmb)", mylist->files[i].d_name, fsize);
								} else { 
									float fsize = atof(mylist->files[i].st_size) / 1024;
									debugLine("%s (%.0fkb)", mylist->files[i].d_name, fsize);
								}
								
							}
						}
					} else {
						debugLine("[ERROR] Retrieving List");
					}
				} else if (debugCurrent() == 1) {
					// CWD
					char cwdir[25];
					err = debugInput(cwdir, "Change to Dir", "", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						if ( strlen(cwdir) != 0 ) ftpCWD(cwdir);
					}
				} else if (debugCurrent() == 2) {
					// RETR
					char retrfile[25] = "\0";
					err = debugInput(retrfile, "Download File", "", CHAR_ALL);
					if ( err != 0 ) { 
						// user did not cancel
						char savefile[25] = "\0";
						err = debugInput(savefile, "Save file to", "ms0:/", CHAR_ALL);
						if ( err != 0 ) { 
							// user did not cancel
							if ( strlen(retrfile) != 0 && strlen(savefile) != 0 ) ftpRETR(savefile, retrfile);
						}
					}; 
				} else if ( debugCurrent() == 3 ) {
					// STOR
					char filedir[25] = "\0";
					err = debugInput(filedir, "Location of file", "ms0:/", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						char sendfile[25] = "\0";
						err = debugInput(sendfile, "File to Send", "", CHAR_ALL);
						if ( err != 0 ) {
							// user did not cancel
							if ( strlen(sendfile) != 0 ) ftpSTOR(filedir, sendfile);
						}
					}
				} else if ( debugCurrent() == 4 ) {
					// PWD
					ftpPWD();
				} else if ( debugCurrent() == 5 ) {
					// MKD
					char mkdir[25];
					err = debugInput(mkdir, "Dir to Create", "", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						if ( strlen(mkdir) != 0 ) ftpMKD(mkdir);
					}
				} else if ( debugCurrent() == 6 ) {
					// RMD
					char rmdir[25];
					err = debugInput(rmdir, "Dir to Remove", "", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						if ( strlen(rmdir) != 0 ) ftpRMD(rmdir);
					}
				} else if ( debugCurrent() == 7 ) {
					// CHMOD
					char chmodfile[25];
					char chmodperm[5];
					err = debugInput(chmodfile, "File to CHMOD", "", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						if ( strlen(chmodfile) != 0 ) {
							err = debugInput(chmodperm, "CHMOD Permissions", "777", CHAR_NUMBERS);
							if ( err != 0 ) {
								// user did not cancel
								if ( strlen(chmodperm) != 0 ) {
									ftpCHMOD(chmodfile, chmodperm);
								}
							}
						}
					}
				} else if ( debugCurrent() == 8 ) {
					// DELE
					char delfile[25];
					err = debugInput(delfile, "Delete File", "", CHAR_ALL);
					if ( err != 0 ) {
						// user did not cancel
						if ( strlen(delfile) != 0 ) ftpDELE(delfile);
					}
				} else if ( debugCurrent() == 9 ) {
					// SYST
					ftpSYST();
				} else if ( debugCurrent() == 10 ) {
					// PASV enable/disable
					if ( ftpPASVEnabled() == 1 ) {
						debugLine("PASV disabled");
						ftpSetPASV(0);
					} else {
						debugLine("PASV enabled");
						ftpSetPASV(1);
					}
				} else if ( debugCurrent() == 11 ) {
					// RECONNECT
					ftpDisconnect();
					err = GetInfo();
					err = AttemptConnect();
				} else if ( debugCurrent() == 12 ) {
					// QUIT
					ftpDisconnect();
					done = 1;
				}
				bPressed = 1;
			}
		}
		
		// delay 100ms for key input if a button was pressed, to avoid multiple presses
		if ( bPressed == 1) {
			sceKernelDelayThread(100 * 1000);
			bPressed = 0;
		}
		sceKernelDelayThread(100 * 1000);
	}

	return 0;
}

int main(void)
{
  int user_thid;
	
	// initialize controls
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	pspDebugScreenInit();

  pspDebugScreenPrintf("PSP-FTP v1.0 by liquid8d\n");
  pspDebugScreenPrintf("Please wait now loading !\n");
  sceKernelDelayThread(2000000); 
	
	SetupCallbacks();
	
  if (nlhLoadDrivers(&module_info) != 0) {
    pspDebugScreenPrintf("unable to load wifi drivers !");
    return 0;
  }

  pspDebugScreenPrintf("Launch main thread !\n");
  sceKernelDelayThread(1000000); 

  user_thid = sceKernelCreateThread( "user_thread", 
     (SceKernelThreadEntry)ftp_user_thread, 0x8, 256*1024, 
     PSP_THREAD_ATTR_USER, 0 );
  if(user_thid >= 0) {
    sceKernelStartThread(user_thid, 0, 0);
    sceKernelWaitThreadEnd(user_thid, NULL);
  }

  sceKernelExitGame();
  return 0;
}

