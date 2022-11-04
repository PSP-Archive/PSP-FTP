#include <pspkernel.h>
#include <pspdebug.h>

#include "loadutil.h"

////////////////////////////////////////////////////////////////////
// SLIME NOTE: Module library linkage trickery
//  in theory shouldn't be necessary
//  in practice appears to be necessary

// SYSTEM ENTRY: sceKernelModuleFromUID
// 1.0 ->	88014318:	27bdffd0	addiu sp,sp,#0xffffffd0
// 1.50 ->	88017308:	27bdffd0	addiu sp,sp,#0xffffffd0

u32* ModuleFromUID(u32 oid)
{
	typedef u32* (*MAP_PROC)(u32);
    MAP_PROC pfnMap;

    u32* addr = (u32*)0x88014318;
    if (*addr != 0x27bdffd0)
    {
	    addr = (u32*)0x88017308;
	    if (*addr != 0x27bdffd0)
        {
            // my_print("ERROR: version error (find)!\n");
            return 0;   // something terribly wrong
        }
    }
    pfnMap = (MAP_PROC)addr;
	return (*pfnMap)(oid);
} 

//REVIEW: use tyranid's new pspDebugFind functions
static u32 FindProcEntry(u32 oid, u32 nid)
{
	u32* modPtr = ModuleFromUID(oid);
    if (modPtr == NULL)
        return 0;
    // extra-anal checks
    if ((((long)modPtr) & 0xFF000000) != 0x88000000)
        return 0;
	if ((modPtr[18] - modPtr[16]) < 40)
        return 0;

	// assume standard library order
    {
		u32* modPtr2 = (u32*)modPtr[16];
		int count = (modPtr2[6] >> 16) & 0xFFFF;
		u32* idPtr = (u32*)modPtr2[7];
		u32* procAddrPtr = idPtr + count;
        int i;
	
		for (i = 0; i < count; i++)
		{
	        if (*idPtr == nid)
	            return (*procAddrPtr);
			idPtr++;
			procAddrPtr++;
	    }
    }
	return 0;
}

static int PatchMyLibraryEntries(SceModuleInfo* modInfoPtr, SceUID oidLib)
{
	//REVIEW: should match single module name
    // this version is dumb and walks all of them (assumes NIDs are unique)

    int nPatched = 0;

    int* stubPtr; // 20 byte structure
    for (stubPtr = modInfoPtr->stub_top;
        stubPtr + 5 <= (int*)modInfoPtr->stub_end;
        stubPtr += 5)
    {
        int count = (stubPtr[2] >> 16) & 0xFFFF;
	    int* idPtr = (int*)stubPtr[3];
	    int* procPtr = (int*)stubPtr[4];

        if (stubPtr[1] != 0x90000)
            continue;   // skip non-lazy loaded modules
	    while (count--)
	    {
	        if (procPtr[0] == 0x54C && procPtr[1] == 0)
	        {
	            // SWI - scan for NID
	            u32 proc = FindProcEntry(oidLib, *idPtr);
	            if (proc != 0)
	            {
	                if (((u32)procPtr & 0xF0000000) != (proc & 0xF0000000))
	                {
						// if not in user space we can't use it
		                // my_print("!! NOT PATCH\n");
	                }
	                else
	                {
	                    u32 val = (proc & 0x03FFFFFF) >> 2;
	                    procPtr[0] = 0x0A000000 | val;
	                    procPtr[1] = 0;
	                    
		                nPatched++;
	                }
	            }
	        }
	        idPtr++;
	        procPtr += 2; // 2 opcodes
        }
    }
    return nPatched;
}

SceUID LoadAndStartAndPatch(SceModuleInfo* modInfoPtr, const char* szFile)
    // return oid or error code
{
	SceUID oid;
	int err;
	int startStatus; // ignored

    oid = sceKernelLoadModule(szFile, 0, NULL);
    if (oid & 0x80000000)
        return oid; // error code

	err = sceKernelStartModule(oid, 0, 0, &startStatus, 0);
	if (err != oid)
        return err;

    PatchMyLibraryEntries(modInfoPtr, oid);
    return oid;
}

// SYSTEM ENTRY: sceKernelIcacheInvalidateAll:
// 1.0 -> 	88054618:	40088000	mfc0 r8,cop0[16]
// 1.50 ->	880584f0:	40088000	mfc0 r8,cop0[16]
void FlushCaches()
{
	typedef void (*VOID_PROC)(void);
    VOID_PROC pfnFlush;
    u32* addr = (u32*)0x88054618;
    if (*addr != 0x40088000)
    {
	    addr = (u32*)0x880584f0;
	    if (*addr != 0x40088000)
        {
            // my_print("ERROR: version error (flush)!\n");
            return;   // something terribly wrong
        }
    }
    pfnFlush = (VOID_PROC)addr;

    sceKernelDcacheWritebackAll();
    (*pfnFlush)();
}

////////////////////////////////////////////////////////////////////
