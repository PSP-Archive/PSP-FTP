# Additional Stubs core and lazy (not all used)

      .set noreorder

    .include "src/common.s"

  STUB_START "sceUtility",0x40010000,0x00060005
    STUB_FUNC 0x4db1e739,sceUtilityNetconfInitStart
    STUB_FUNC 0xf88155f6,sceUtilityNetconfShutdownStart
    STUB_FUNC 0x91e70e35,sceUtilityNetconfUpdate
    STUB_FUNC 0x6332aa39,sceUtilityNetconfGetStatus
    STUB_FUNC 0x5eee6548,sceUtilityCheckNetParam
    STUB_FUNC 0x434d4b3a,sceUtilityGetNetParam
  STUB_END

  STUB_START "sceNet",0x90000,0x00050005 
    STUB_FUNC 0x39af39a6,sceNetInit 
    STUB_FUNC 0x281928a9,sceNetTerm 
    STUB_FUNC 0xad6844c6,sceNetThreadAbort 
    STUB_FUNC 0x0bf0a3ae,sceNetGetLocalEtherAddr 
    STUB_FUNC 0xcc393e48,sceNetGetMallocStat 
  STUB_END 

  STUB_START "sceNetInet",0x90000,0x001c0005 
    STUB_FUNC 0x17943399,sceNetInetInit 
    STUB_FUNC 0xa9ed66b9,sceNetInetTerm 
    STUB_FUNC 0xdb094e1b,sceNetInetAccept 
    STUB_FUNC 0x1a33f9ae,sceNetInetBind 
    STUB_FUNC 0x8d7284ea,sceNetInetClose 
    STUB_FUNC 0x805502dd,sceNetInetCloseWithRST 
    STUB_FUNC 0x410b34aa,sceNetInetConnect 
    STUB_FUNC 0xe247b6d6,sceNetInetGetpeername 
    STUB_FUNC 0x162e6fd5,sceNetInetGetsockname 
    STUB_FUNC 0x4a114c7c,sceNetInetGetsockopt 
    STUB_FUNC 0xd10a1a7a,sceNetInetListen 
    STUB_FUNC 0xfaabb1dd,sceNetInetPoll 
    STUB_FUNC 0xcda85c99,sceNetInetRecv 
    STUB_FUNC 0xc91142e4,sceNetInetRecvfrom 
    STUB_FUNC 0xeece61d2,sceNetInetRecvmsg 
    STUB_FUNC 0x5be8d595,sceNetInetSelect 
    STUB_FUNC 0x7aa671bc,sceNetInetSend 
    STUB_FUNC 0x05038fc7,sceNetInetSendto 
    STUB_FUNC 0x774e36f4,sceNetInetSendmsg 
    STUB_FUNC 0x2fe71fe7,sceNetInetSetsockopt 
    STUB_FUNC 0x4cfe4e56,sceNetInetShutdown 
    STUB_FUNC 0x8b7b220f,sceNetInetSocket 
    STUB_FUNC 0x80a21abd,sceNetInetSocketAbort 
    STUB_FUNC 0xfbabe411,sceNetInetGetErrno 
    STUB_FUNC 0xb75d5b0a,sceNetInetInetAddr 
    STUB_FUNC 0x1bdf5d13,sceNetInetInetAton 
    STUB_FUNC 0xd0792666,sceNetInetInetNtop 
    STUB_FUNC 0xe30b8c19,sceNetInetInetPton 
  STUB_END 

  STUB_START "sceNetAdhoc",0x90000,0x00190005 
    STUB_FUNC 0xe1d621d7,sceNetAdhocInit  
    STUB_FUNC 0xa62c6f57,sceNetAdhocTerm  
    STUB_FUNC 0x7a662d6b,sceNetAdhocPollSocket  
    STUB_FUNC 0x73bfd52d,sceNetAdhocSetSocketAlert  
    STUB_FUNC 0x4d2ce199,sceNetAdhocGetSocketAlert  
    STUB_FUNC 0x6f92741b,sceNetAdhocPdpCreate  
    STUB_FUNC 0xabed3790,sceNetAdhocPdpSend  
    STUB_FUNC 0xdfe53e03,sceNetAdhocPdpRecv  
    STUB_FUNC 0x7f27bb5e,sceNetAdhocPdpDelete  
    STUB_FUNC 0xc7c1fc57,sceNetAdhocGetPdpStat  
    STUB_FUNC 0x877f6d66,sceNetAdhocPtpOpen  
    STUB_FUNC 0xfc6fc07b,sceNetAdhocPtpConnect  
    STUB_FUNC 0xe08bdac1,sceNetAdhocPtpListen  
    STUB_FUNC 0x9df81198,sceNetAdhocPtpAccept  
    STUB_FUNC 0x4da4c788,sceNetAdhocPtpSend  
    STUB_FUNC 0x8bea2b3e,sceNetAdhocPtpRecv  
    STUB_FUNC 0x9ac2eeac,sceNetAdhocPtpFlush  
    STUB_FUNC 0x157e6225,sceNetAdhocPtpClose  
    STUB_FUNC 0xb9685118,sceNetAdhocGetPtpStat  
    STUB_FUNC 0x7f75c338,sceNetAdhocGameModeCreateMaster  
    STUB_FUNC 0x3278ab0c,sceNetAdhocGameModeCreateReplica  
    STUB_FUNC 0x98c204c8,sceNetAdhocGameModeUpdateMaster  
    STUB_FUNC 0xfa324b4e,sceNetAdhocGameModeUpdateReplica  
    STUB_FUNC 0xa0229362,sceNetAdhocGameModeDeleteMaster  
    STUB_FUNC 0x0b2228e9,sceNetAdhocGameModeDeleteReplica 
  STUB_END 

  STUB_START "sceNetAdhocctl",0x90000,0x00140005 
    STUB_FUNC 0xe26f226e,sceNetAdhocctlInit  
    STUB_FUNC 0x9d689e13,sceNetAdhocctlTerm  
    STUB_FUNC 0x0ad043ed,sceNetAdhocctlConnect  
    STUB_FUNC 0xec0635c1,sceNetAdhocctlCreate  
    STUB_FUNC 0x5e7f79c9,sceNetAdhocctlJoin  
    STUB_FUNC 0x08fff7a0,sceNetAdhocctlScan  
    STUB_FUNC 0x34401d65,sceNetAdhocctlDisconnect  
    STUB_FUNC 0x20b317a0,sceNetAdhocctlAddHandler  
    STUB_FUNC 0x6402490b,sceNetAdhocctlDelHandler  
    STUB_FUNC 0x75ecd386,sceNetAdhocctlGetState  
    STUB_FUNC 0x362cbe8f,sceNetAdhocctlGetAdhocId  
    STUB_FUNC 0xe162cb14,sceNetAdhocctlGetPeerList  
    STUB_FUNC 0x99560abe,sceNetAdhocctlGetAddrByName  
    STUB_FUNC 0x8916c003,sceNetAdhocctlGetNameByAddr  
    STUB_FUNC 0xded9d28e,sceNetAdhocctlGetParameter  
    STUB_FUNC 0x81aee1be,sceNetAdhocctlGetScanInfo  
    STUB_FUNC 0xa5c055ce,sceNetAdhocctlCreateEnterGameMode  
    STUB_FUNC 0x1ff89745,sceNetAdhocctlJoinEnterGameMode  
    STUB_FUNC 0xcf8e084d,sceNetAdhocctlExitGameMode  
    STUB_FUNC 0x5a014ce0,sceNetAdhocctlGetGameModeInfo 
  STUB_END 

  STUB_START "sceNetResolver",0x90000,0x00070005 
    STUB_FUNC 0xf3370e61,sceNetResolverInit  
    STUB_FUNC 0x6138194a,sceNetResolverTerm  
    STUB_FUNC 0x244172af,sceNetResolverCreate  
    STUB_FUNC 0x94523e09,sceNetResolverDelete  
    STUB_FUNC 0x224c5f44,sceNetResolverStartNtoA  
    STUB_FUNC 0x629e2fb7,sceNetResolverStartAtoN  
    STUB_FUNC 0x808f6063,sceNetResolverStop 
  STUB_END 

  STUB_START "sceNetApctl",0x90000,0x00080005 
    STUB_FUNC 0xe2f91f9b,sceNetApctlInit  
    STUB_FUNC 0xb3edd0ec,sceNetApctlTerm  
    STUB_FUNC 0x2befdf23,sceNetApctlGetInfo  
    STUB_FUNC 0x8abadd51,sceNetApctlAddHandler  
    STUB_FUNC 0x5963991b,sceNetApctlDelHandler  
    STUB_FUNC 0xcfb957c6,sceNetApctlConnect  
    STUB_FUNC 0x24fe91a1,sceNetApctlDisconnect  
    STUB_FUNC 0x5deac81b,sceNetApctlGetState 
  STUB_END 

