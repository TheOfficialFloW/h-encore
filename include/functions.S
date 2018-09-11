/* functions.S -- imported functions
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

// SceJpegUser
.equ sceJpegDecodeMJpegYCbCr, 0x8102f294
.equ sceJpegGetOutputInfo, 0x8102f2a4
.equ sceJpegInitMJpegWithParam, 0x8102f2b4
.equ sceJpegCsc, 0x8102f2c4
.equ sceJpegFinishMJpeg, 0x8102f2d4
.equ sceJpegMJpegCsc, 0x8102f2e4

// SceJpegEncUser
.equ sceJpegEncoderSetOutputAddr, 0x8102f2f4
.equ sceJpegEncoderGetContextSize, 0x8102f304
.equ sceJpegEncoderInitWithParam, 0x8102f314
.equ sceJpegEncoderCsc, 0x8102f324
.equ sceJpegEncoderSetValidRegion, 0x8102f334
.equ sceJpegEncoderSetCompressionRatio, 0x8102f344
.equ sceJpegEncoderEncode, 0x8102f354
.equ sceJpegEncoderEnd, 0x8102f364

// SceGxm
.equ sceGxmMapFragmentUsseMemory, 0x8102f374
.equ sceGxmShaderPatcherCreate, 0x8102f384
.equ sceGxmFinish, 0x8102f394
.equ sceGxmUnmapVertexUsseMemory, 0x8102f3a4
.equ sceGxmDestroyRenderTarget, 0x8102f3b4
.equ sceGxmTextureGetWidth, 0x8102f3c4
.equ sceGxmTextureSetVAddrMode, 0x8102f3d4
.equ sceGxmSetFrontDepthFunc, 0x8102f3e4
.equ sceGxmTextureSetMipFilter, 0x8102f3f4
.equ sceGxmCreateRenderTarget, 0x8102f404
.equ sceGxmProgramFindParameterByName, 0x8102f414
.equ sceGxmSetFragmentTexture, 0x8102f424
.equ sceGxmShaderPatcherRegisterProgram, 0x8102f434
.equ sceGxmSetVertexProgram, 0x8102f444
.equ sceGxmTextureSetMinFilter, 0x8102f454
.equ sceGxmTextureSetUAddrMode, 0x8102f464
.equ sceGxmTransferFinish, 0x8102f474
.equ sceGxmTextureInitLinear, 0x8102f484
.equ sceGxmShaderPatcherCreateFragmentProgram, 0x8102f494
.equ sceGxmTextureGetData, 0x8102f4a4
.equ sceGxmTextureGetHeight, 0x8102f4b4
.equ sceGxmProgramParameterGetResourceIndex, 0x8102f4c4
.equ sceGxmTransferCopy, 0x8102f4d4
.equ sceGxmSetUniformDataF, 0x8102f4e4
.equ sceGxmTextureInitLinearStrided, 0x8102f4f4
.equ sceGxmTransferFill, 0x8102f504
.equ sceGxmSyncObjectCreate, 0x8102f514
.equ sceGxmReserveFragmentDefaultUniformBuffer, 0x8102f524
.equ sceGxmUnmapFragmentUsseMemory, 0x8102f534
.equ sceGxmUnmapMemory, 0x8102f544
.equ sceGxmBeginScene, 0x8102f554
.equ sceGxmSyncObjectDestroy, 0x8102f564
.equ sceGxmSetVertexStream, 0x8102f574
.equ sceGxmShaderPatcherGetProgramFromId, 0x8102f584
.equ sceGxmShaderPatcherReleaseVertexProgram, 0x8102f594
.equ sceGxmSetFragmentProgram, 0x8102f5a4
.equ sceGxmSetBackDepthFunc, 0x8102f5b4
.equ sceGxmInitialize, 0x8102f5c4
.equ sceGxmTerminate, 0x8102f5d4
.equ sceGxmShaderPatcherCreateVertexProgram, 0x8102f5e4
.equ sceGxmDisplayQueueFinish, 0x8102f5f4
.equ sceGxmDraw, 0x8102f604
.equ sceGxmShaderPatcherReleaseFragmentProgram, 0x8102f614
.equ sceGxmMapMemory, 0x8102f624
.equ sceGxmDepthStencilSurfaceInit, 0x8102f634
.equ sceGxmTransferDownscale, 0x8102f644
.equ sceGxmCreateContext, 0x8102f654
.equ sceGxmShaderPatcherDestroy, 0x8102f664
.equ sceGxmDisplayQueueAddEntry, 0x8102f674
.equ sceGxmColorSurfaceInit, 0x8102f684
.equ sceGxmDestroyContext, 0x8102f694
.equ sceGxmShaderPatcherUnregisterProgram, 0x8102f6a4
.equ sceGxmMapVertexUsseMemory, 0x8102f6b4
.equ sceGxmTextureSetMagFilter, 0x8102f6c4
.equ sceGxmEndScene, 0x8102f6d4

// SceDisplayUser
.equ sceDisplaySetFrameBuf, 0x8102f6e4

// SceDisplay
.equ sceDisplayWaitVblankStart, 0x8102f6f4
.equ sceDisplayWaitSetFrameBufMulti, 0x8102f704
.equ sceDisplayWaitSetFrameBuf, 0x8102f714

// SceCtrl
.equ sceCtrlReadBufferPositive, 0x8102f724
.equ sceCtrlSetSamplingMode, 0x8102f734

// SceAtrac
.equ sceAtracReleaseHandle, 0x8102f744
.equ sceAtracCreateDecoderGroup, 0x8102f754
.equ sceAtracSetOutputSamples, 0x8102f764
.equ sceAtracQueryDecoderGroupMemSize, 0x8102f774
.equ sceAtracSetSubBuffer, 0x8102f784
.equ sceAtracDecode, 0x8102f794
.equ sceAtracAddStreamData, 0x8102f7a4
.equ sceAtracSetLoopNum, 0x8102f7b4
.equ sceAtracGetContentInfo, 0x8102f7c4
.equ sceAtracDeleteDecoderGroup, 0x8102f7d4
.equ sceAtracIsSubBufferNeeded, 0x8102f7e4
.equ sceAtracGetNextOutputPosition, 0x8102f7f4
.equ sceAtracSetDataAndAcquireHandle, 0x8102f804
.equ sceAtracResetNextOutputPosition, 0x8102f814
.equ sceAtracGetStreamInfo, 0x8102f824
.equ sceAtracGetSubBufferInfo, 0x8102f834

// SceAudio
.equ sceAudioOutOutput, 0x8102f844
.equ sceAudioOutOpenPort, 0x8102f854
.equ sceAudioOutSetVolume, 0x8102f864
.equ sceAudioOutReleasePort, 0x8102f874

// SceSysmodule
.equ sceSysmoduleUnloadModule, 0x8102f884
.equ sceSysmoduleLoadModule, 0x8102f894

// SceRtcUser
.equ sceRtcGetCurrentClockLocalTime, 0x8102f8a4

// ScePvf
.equ scePvfFindOptimumFont, 0x8102f8b4
.equ scePvfSetSkewValue, 0x8102f8c4
.equ scePvfSetEmboldenRate, 0x8102f8d4
.equ scePvfNewLib, 0x8102f8e4
.equ scePvfGetCharGlyphImage_Clip, 0x8102f8f4
.equ scePvfGetCharInfo, 0x8102f904
.equ scePvfGetFontInfo, 0x8102f914
.equ scePvfSetResolution, 0x8102f924
.equ scePvfClose, 0x8102f934
.equ scePvfSetEM, 0x8102f944
.equ scePvfDoneLib, 0x8102f954
.equ scePvfOpen, 0x8102f964
.equ scePvfSetCharSize, 0x8102f974

// SceTouch
.equ sceTouchRead, 0x8102f984
.equ sceTouchSetSamplingState, 0x8102f994

// SceSystemGesture
.equ sceSystemGestureGetTouchEventsCount, 0x8102f9a4
.equ sceSystemGestureGetTouchEventByEventID, 0x8102f9b4
.equ sceSystemGestureInitializePrimitiveTouchRecognizer, 0x8102f9c4
.equ sceSystemGestureGetTouchEventByIndex, 0x8102f9d4
.equ sceSystemGestureUpdateTouchRecognizer, 0x8102f9e4
.equ sceSystemGestureFinalizePrimitiveTouchRecognizer, 0x8102f9f4
.equ sceSystemGestureCreateTouchRecognizer, 0x8102fa04
.equ sceSystemGestureUpdatePrimitiveTouchRecognizer, 0x8102fa14

// SceAppUtil
.equ sceAppUtilSaveDataDataSave, 0x8102fa24
.equ sceAppUtilSaveDataSlotGetParam, 0x8102fa34
.equ sceAppUtilShutdown, 0x8102fa44
.equ sceAppUtilSaveDataDataRemove, 0x8102fa54
.equ sceAppUtilInit, 0x8102fa64

// SceCommonDialog
.equ sceSaveDataDialogContinue, 0x8102fa74
.equ sceSaveDataDialogTerm, 0x8102fa84
.equ sceMsgDialogGetStatus, 0x8102fa94
.equ sceSaveDataDialogSubClose, 0x8102faa4
.equ sceSaveDataDialogFinish, 0x8102fab4
.equ sceSaveDataDialogGetStatus, 0x8102fac4
.equ sceMsgDialogInit, 0x8102fad4
.equ sceMsgDialogTerm, 0x8102fae4
.equ sceCommonDialogUpdate, 0x8102faf4
.equ sceNpTrophySetupDialogInit, 0x8102fb04
.equ sceNpTrophySetupDialogTerm, 0x8102fb14
.equ sceSaveDataDialogGetResult, 0x8102fb24
.equ sceSaveDataDialogGetSubStatus, 0x8102fb34
.equ sceMsgDialogGetResult, 0x8102fb44
.equ sceSaveDataDialogInit, 0x8102fb54
.equ sceMsgDialogClose, 0x8102fb64
.equ sceNpTrophySetupDialogGetStatus, 0x8102fb74
.equ sceNpTrophySetupDialogGetResult, 0x8102fb84

// SceNpTrophy
.equ sceNpTrophyInit, 0x8102fb94
.equ sceNpTrophyCreateHandle, 0x8102fba4
.equ sceNpTrophyDestroyContext, 0x8102fbb4
.equ sceNpTrophyUnlockTrophy, 0x8102fbc4
.equ sceNpTrophyGetGameInfo, 0x8102fbd4
.equ sceNpTrophyTerm, 0x8102fbe4
.equ sceNpTrophyCreateContext, 0x8102fbf4
.equ sceNpTrophyGetTrophyUnlockState, 0x8102fc04
.equ sceNpTrophyAbortHandle, 0x8102fc14
.equ sceNpTrophyDestroyHandle, 0x8102fc24

// SceNpManager
.equ sceNpInit, 0x8102fc34
.equ sceNpTerm, 0x8102fc44

// SceAvPlayer
.equ sceAvPlayerIsActive, 0x8102fc54
.equ sceAvPlayerInit, 0x8102fc64
.equ sceAvPlayerClose, 0x8102fc74
.equ sceAvPlayerGetVideoData, 0x8102fc84
.equ sceAvPlayerGetAudioData, 0x8102fc94
.equ sceAvPlayerAddSource, 0x8102fca4
.equ sceAvPlayerStop, 0x8102fcb4

// SceLibc
.equ strstr, 0x8102fcc4
.equ abort, 0x8102fcd4
.equ __cxa_atexit, 0x8102fce4
.equ _sceLdTlsRegisterModuleInfo, 0x8102fcf4
.equ free, 0x8102fd04
.equ memset, 0x8102fd14
.equ memcpy, 0x8102fd24
.equ malloc, 0x8102fd34
.equ exit, 0x8102fd44
.equ fgetwc, 0x8102fd54
.equ strlen, 0x8102fd64
.equ abs, 0x8102fd74
.equ printf, 0x8102fd84
.equ strncpy, 0x8102fd94
.equ snprintf, 0x8102fda4
.equ wcslen, 0x8102fdb4
.equ memalign, 0x8102fdc4
.equ __cxa_set_dso_handle_main, 0x8102fdd4
.equ rand, 0x8102fde4
.equ strncmp, 0x8102fdf4
.equ fclose, 0x8102fe04
.equ fopen, 0x8102fe14

// SceLibm
.equ _FSinx, 0x8102fe24

// SceLibstdcxx
.equ _ZdlPv, 0x8102fe34
.equ _Znwj, 0x8102fe44

// SceLibKernel
.equ sceKernelUnlockLwMutex, 0x8102fe54
.equ sceClibMemcpy, 0x8102fe64
.equ sceKernelDeleteLwMutex, 0x8102fe74
.equ __sce_aeabi_idiv0, 0x8102fe84
.equ sceKernelLockLwMutex, 0x8102fe94
.equ sceKernelGetProcessTime, 0x8102fea4
.equ sceClibMemset, 0x8102feb4
.equ sceIoOpen, 0x8102fec4
.equ sceClibMemmove, 0x8102fed4
.equ sceKernelCreateEventFlag, 0x8102fee4
.equ sceIoLseek, 0x8102fef4
.equ sceIoGetstat, 0x8102ff04
.equ sceKernelCreateThread, 0x8102ff14
.equ sceKernelGetThreadExitStatus, 0x8102ff24
.equ sceKernelCreateLwMutex, 0x8102ff34
.equ sceKernelWaitThreadEnd, 0x8102ff44
.equ sceKernelStartThread, 0x8102ff54
.equ sceKernelLoadStartModule, 0x8102ff64

// SceIofilemgr
.equ sceIoLseek32, 0x8102ff74
.equ sceIoClose, 0x8102ff84
.equ sceIoRead, 0x8102ff94

// SceProcessmgr
.equ sceKernelPowerTick, 0x8102ffa4

// SceSysmem
.equ sceKernelFindMemBlockByAddr, 0x8102ffb4
.equ sceKernelFreeMemBlock, 0x8102ffc4
.equ sceKernelGetMemBlockBase, 0x8102ffd4
.equ sceKernelAllocMemBlock, 0x8102ffe4

// SceThreadmgr
.equ sceKernelDeleteThread, 0x8102fff4
.equ sceKernelExitDeleteThread, 0x81030004
.equ sceKernelDelayThread, 0x81030014
.equ sceKernelDeleteEventFlag, 0x81030024
.equ sceKernelSetEventFlag, 0x81030034

// SceThreadmgrCoredumpTime
.equ sceKernelExitThread, 0x81030044
