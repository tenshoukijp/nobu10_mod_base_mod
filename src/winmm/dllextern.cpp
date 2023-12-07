#include <windows.h>
#include <string>
#include <map>

#include "output_debug_stream.h"
#include "loadmod.h"
#include "on_event.h"
#include "mmsystem.h"
#include "file_attribute.h"

using namespace std;


HINSTANCE hOriginalDll;
extern HWND hNB10Wnd;


FARPROC p_NONAME1;
FARPROC p_mciExecute;
FARPROC p_CloseDriver;
FARPROC p_DefDriverProc;
FARPROC p_DriverCallback;
FARPROC p_DrvGetModuleHandle;
FARPROC p_GetDriverModuleHandle;
FARPROC p_NotifyCallbackData;
FARPROC p_OpenDriver;
FARPROC p_PlaySound;
FARPROC p_PlaySoundA;
FARPROC p_PlaySoundW;
FARPROC p_SendDriverMessage;
FARPROC p_WOW32DriverCallback;
FARPROC p_WOW32ResolveMultiMediaHandle;
FARPROC p_WOWAppExit;
FARPROC p_aux32Message;
FARPROC p_auxGetDevCapsA;
FARPROC p_auxGetDevCapsW;
FARPROC p_auxGetNumDevs;
FARPROC p_auxGetVolume;
FARPROC p_auxOutMessage;
FARPROC p_auxSetVolume;
FARPROC p_joy32Message;
FARPROC p_joyConfigChanged;
FARPROC p_joyGetDevCapsA;
FARPROC p_joyGetDevCapsW;
FARPROC p_joyGetNumDevs;
FARPROC p_joyGetPos;
FARPROC p_joyGetPosEx;
FARPROC p_joyGetThreshold;
FARPROC p_joyReleaseCapture;
FARPROC p_joySetCapture;
FARPROC p_joySetThreshold;
FARPROC p_mci32Message;
FARPROC p_mciDriverNotify;
FARPROC p_mciDriverYield;
FARPROC p_mciFreeCommandResource;
FARPROC p_mciGetCreatorTask;
FARPROC p_mciGetDeviceIDA;
FARPROC p_mciGetDeviceIDFromElementIDA;
FARPROC p_mciGetDeviceIDFromElementIDW;
FARPROC p_mciGetDeviceIDW;
FARPROC p_mciGetDriverData;
FARPROC p_mciGetErrorStringA;
FARPROC p_mciGetErrorStringW;
FARPROC p_mciGetYieldProc;
FARPROC p_mciLoadCommandResource;
FARPROC p_mciSendCommandA;
FARPROC p_mciSendCommandW;
FARPROC p_mciSendStringA;
FARPROC p_mciSendStringW;
FARPROC p_mciSetDriverData;
FARPROC p_mciSetYieldProc;
FARPROC p_mid32Message;
FARPROC p_midiConnect;
FARPROC p_midiDisconnect;
FARPROC p_midiInAddBuffer;
FARPROC p_midiInClose;
FARPROC p_midiInGetDevCapsA;
FARPROC p_midiInGetDevCapsW;
FARPROC p_midiInGetErrorTextA;
FARPROC p_midiInGetErrorTextW;
FARPROC p_midiInGetID;
FARPROC p_midiInGetNumDevs;
FARPROC p_midiInMessage;
FARPROC p_midiInOpen;
FARPROC p_midiInPrepareHeader;
FARPROC p_midiInReset;
FARPROC p_midiInStart;
FARPROC p_midiInStop;
FARPROC p_midiInUnprepareHeader;
FARPROC p_midiOutCacheDrumPatches;
FARPROC p_midiOutCachePatches;
FARPROC p_midiOutClose;
FARPROC p_midiOutGetDevCapsA;
FARPROC p_midiOutGetDevCapsW;
FARPROC p_midiOutGetErrorTextA;
FARPROC p_midiOutGetErrorTextW;
FARPROC p_midiOutGetID;
FARPROC p_midiOutGetNumDevs;
FARPROC p_midiOutGetVolume;
FARPROC p_midiOutLongMsg;
FARPROC p_midiOutMessage;
FARPROC p_midiOutOpen;
FARPROC p_midiOutPrepareHeader;
FARPROC p_midiOutReset;
FARPROC p_midiOutSetVolume;
FARPROC p_midiOutShortMsg;
FARPROC p_midiOutUnprepareHeader;
FARPROC p_midiStreamClose;
FARPROC p_midiStreamOpen;
FARPROC p_midiStreamOut;
FARPROC p_midiStreamPause;
FARPROC p_midiStreamPosition;
FARPROC p_midiStreamProperty;
FARPROC p_midiStreamRestart;
FARPROC p_midiStreamStop;
FARPROC p_mixerClose;
FARPROC p_mixerGetControlDetailsA;
FARPROC p_mixerGetControlDetailsW;
FARPROC p_mixerGetDevCapsA;
FARPROC p_mixerGetDevCapsW;
FARPROC p_mixerGetID;
FARPROC p_mixerGetLineControlsA;
FARPROC p_mixerGetLineControlsW;
FARPROC p_mixerGetLineInfoA;
FARPROC p_mixerGetLineInfoW;
FARPROC p_mixerGetNumDevs;
FARPROC p_mixerMessage;
FARPROC p_mixerOpen;
FARPROC p_mixerSetControlDetails;
FARPROC p_mmDrvInstall;
FARPROC p_mmGetCurrentTask;
FARPROC p_mmTaskBlock;
FARPROC p_mmTaskCreate;
FARPROC p_mmTaskSignal;
FARPROC p_mmTaskYield;
FARPROC p_mmioAdvance;
FARPROC p_mmioAscend;
FARPROC p_mmioClose;
FARPROC p_mmioCreateChunk;
FARPROC p_mmioDescend;
FARPROC p_mmioFlush;
FARPROC p_mmioGetInfo;
FARPROC p_mmioInstallIOProcA;
FARPROC p_mmioInstallIOProcW;
FARPROC p_mmioOpenA;
FARPROC p_mmioOpenW;
FARPROC p_mmioRead;
FARPROC p_mmioRenameA;
FARPROC p_mmioRenameW;
FARPROC p_mmioSeek;
FARPROC p_mmioSendMessage;
FARPROC p_mmioSetBuffer;
FARPROC p_mmioSetInfo;
FARPROC p_mmioStringToFOURCCA;
FARPROC p_mmioStringToFOURCCW;
FARPROC p_mmioWrite;
FARPROC p_mmsystemGetVersion;
FARPROC p_mod32Message;
FARPROC p_mxd32Message;
FARPROC p_sndPlaySoundA;
FARPROC p_sndPlaySoundW;
FARPROC p_tid32Message;
FARPROC p_timeBeginPeriod;
FARPROC p_timeEndPeriod;
FARPROC p_timeGetDevCaps;
FARPROC p_timeGetSystemTime;
FARPROC p_timeGetTime;
FARPROC p_timeKillEvent;
FARPROC p_timeSetEvent;
FARPROC p_waveInAddBuffer;
FARPROC p_waveInClose;
FARPROC p_waveInGetDevCapsA;
FARPROC p_waveInGetDevCapsW;
FARPROC p_waveInGetErrorTextA;
FARPROC p_waveInGetErrorTextW;
FARPROC p_waveInGetID;
FARPROC p_waveInGetNumDevs;
FARPROC p_waveInGetPosition;
FARPROC p_waveInMessage;
FARPROC p_waveInOpen;
FARPROC p_waveInPrepareHeader;
FARPROC p_waveInReset;
FARPROC p_waveInStart;
FARPROC p_waveInStop;
FARPROC p_waveInUnprepareHeader;
FARPROC p_waveOutBreakLoop;
FARPROC p_waveOutClose;
FARPROC p_waveOutGetDevCapsA;
FARPROC p_waveOutGetDevCapsW;
FARPROC p_waveOutGetErrorTextA;
FARPROC p_waveOutGetErrorTextW;
FARPROC p_waveOutGetID;
FARPROC p_waveOutGetNumDevs;
FARPROC p_waveOutGetPitch;
FARPROC p_waveOutGetPlaybackRate;
FARPROC p_waveOutGetPosition;
FARPROC p_waveOutGetVolume;
FARPROC p_waveOutMessage;
FARPROC p_waveOutOpen;
FARPROC p_waveOutPause;
FARPROC p_waveOutPrepareHeader;
FARPROC p_waveOutReset;
FARPROC p_waveOutRestart;
FARPROC p_waveOutSetPitch;
FARPROC p_waveOutSetPlaybackRate;
FARPROC p_waveOutSetVolume;
FARPROC p_waveOutUnprepareHeader;
FARPROC p_waveOutWrite;
FARPROC p_wid32Message;
FARPROC p_wod32Message;


extern "C" {
    __declspec(naked) void WINAPI d_NONAME1() { _asm { jmp p_NONAME1 } }
    __declspec(naked) void WINAPI d_mciExecute() { _asm { jmp p_mciExecute } }
    __declspec(naked) void WINAPI d_CloseDriver() { _asm { jmp p_CloseDriver } }
    __declspec(naked) void WINAPI d_DefDriverProc() { _asm { jmp p_DefDriverProc } }
    __declspec(naked) void WINAPI d_DriverCallback() { _asm { jmp p_DriverCallback } }
    __declspec(naked) void WINAPI d_DrvGetModuleHandle() { _asm { jmp p_DrvGetModuleHandle } }
    __declspec(naked) void WINAPI d_GetDriverModuleHandle() { _asm { jmp p_GetDriverModuleHandle } }
    __declspec(naked) void WINAPI d_NotifyCallbackData() { _asm { jmp p_NotifyCallbackData } }
    __declspec(naked) void WINAPI d_OpenDriver() { _asm { jmp p_OpenDriver } }
    __declspec(naked) void WINAPI d_PlaySound() { _asm { jmp p_PlaySound } }
    __declspec(naked) void WINAPI d_PlaySoundA() { _asm { jmp p_PlaySoundA } }
    __declspec(naked) void WINAPI d_PlaySoundW() { _asm { jmp p_PlaySoundW } }
    __declspec(naked) void WINAPI d_SendDriverMessage() { _asm { jmp p_SendDriverMessage } }
    __declspec(naked) void WINAPI d_WOW32DriverCallback() { _asm { jmp p_WOW32DriverCallback } }
    __declspec(naked) void WINAPI d_WOW32ResolveMultiMediaHandle() { _asm { jmp p_WOW32ResolveMultiMediaHandle } }
    __declspec(naked) void WINAPI d_WOWAppExit() { _asm { jmp p_WOWAppExit } }
    __declspec(naked) void WINAPI d_aux32Message() { _asm { jmp p_aux32Message } }
    __declspec(naked) void WINAPI d_auxGetDevCapsA() { _asm { jmp p_auxGetDevCapsA } }
    __declspec(naked) void WINAPI d_auxGetDevCapsW() { _asm { jmp p_auxGetDevCapsW } }
    __declspec(naked) void WINAPI d_auxGetNumDevs() { _asm { jmp p_auxGetNumDevs } }
    __declspec(naked) void WINAPI d_auxGetVolume() { _asm { jmp p_auxGetVolume } }
    __declspec(naked) void WINAPI d_auxOutMessage() { _asm { jmp p_auxOutMessage } }
    __declspec(naked) void WINAPI d_auxSetVolume() { _asm { jmp p_auxSetVolume } }
    __declspec(naked) void WINAPI d_joy32Message() { _asm { jmp p_joy32Message } }
    __declspec(naked) void WINAPI d_joyConfigChanged() { _asm { jmp p_joyConfigChanged } }
    __declspec(naked) void WINAPI d_joyGetDevCapsA() { _asm { jmp p_joyGetDevCapsA } }
    __declspec(naked) void WINAPI d_joyGetDevCapsW() { _asm { jmp p_joyGetDevCapsW } }
    __declspec(naked) void WINAPI d_joyGetNumDevs() { _asm { jmp p_joyGetNumDevs } }
    __declspec(naked) void WINAPI d_joyGetPos() { _asm { jmp p_joyGetPos } }
    __declspec(naked) void WINAPI d_joyGetPosEx() { _asm { jmp p_joyGetPosEx } }
    __declspec(naked) void WINAPI d_joyGetThreshold() { _asm { jmp p_joyGetThreshold } }
    __declspec(naked) void WINAPI d_joyReleaseCapture() { _asm { jmp p_joyReleaseCapture } }
    __declspec(naked) void WINAPI d_joySetCapture() { _asm { jmp p_joySetCapture } }
    __declspec(naked) void WINAPI d_joySetThreshold() { _asm { jmp p_joySetThreshold } }
    __declspec(naked) void WINAPI d_mci32Message() { _asm { jmp p_mci32Message } }
    __declspec(naked) void WINAPI d_mciDriverNotify() { _asm { jmp p_mciDriverNotify } }
    __declspec(naked) void WINAPI d_mciDriverYield() { _asm { jmp p_mciDriverYield } }
    __declspec(naked) void WINAPI d_mciFreeCommandResource() { _asm { jmp p_mciFreeCommandResource } }
    __declspec(naked) void WINAPI d_mciGetCreatorTask() { _asm { jmp p_mciGetCreatorTask } }
    __declspec(naked) void WINAPI d_mciGetDeviceIDA() { _asm { jmp p_mciGetDeviceIDA } }
    __declspec(naked) void WINAPI d_mciGetDeviceIDFromElementIDA() { _asm { jmp p_mciGetDeviceIDFromElementIDA } }
    __declspec(naked) void WINAPI d_mciGetDeviceIDFromElementIDW() { _asm { jmp p_mciGetDeviceIDFromElementIDW } }
    __declspec(naked) void WINAPI d_mciGetDeviceIDW() { _asm { jmp p_mciGetDeviceIDW } }
    __declspec(naked) void WINAPI d_mciGetDriverData() { _asm { jmp p_mciGetDriverData } }
    __declspec(naked) void WINAPI d_mciGetErrorStringA() { _asm { jmp p_mciGetErrorStringA } }
    __declspec(naked) void WINAPI d_mciGetErrorStringW() { _asm { jmp p_mciGetErrorStringW } }
    __declspec(naked) void WINAPI d_mciGetYieldProc() { _asm { jmp p_mciGetYieldProc } }
    __declspec(naked) void WINAPI d_mciLoadCommandResource() { _asm { jmp p_mciLoadCommandResource } }
    __declspec(naked) void WINAPI d_mciSendCommandA() { _asm { jmp p_mciSendCommandA } }
    __declspec(naked) void WINAPI d_mciSendCommandW() { _asm { jmp p_mciSendCommandW } }
    __declspec(naked) void WINAPI d_mciSendStringA() { _asm { jmp p_mciSendStringA } }
    __declspec(naked) void WINAPI d_mciSendStringW() { _asm { jmp p_mciSendStringW } }
    __declspec(naked) void WINAPI d_mciSetDriverData() { _asm { jmp p_mciSetDriverData } }
    __declspec(naked) void WINAPI d_mciSetYieldProc() { _asm { jmp p_mciSetYieldProc } }
    __declspec(naked) void WINAPI d_mid32Message() { _asm { jmp p_mid32Message } }
    __declspec(naked) void WINAPI d_midiConnect() { _asm { jmp p_midiConnect } }
    __declspec(naked) void WINAPI d_midiDisconnect() { _asm { jmp p_midiDisconnect } }
    __declspec(naked) void WINAPI d_midiInAddBuffer() { _asm { jmp p_midiInAddBuffer } }
    __declspec(naked) void WINAPI d_midiInClose() { _asm { jmp p_midiInClose } }
    __declspec(naked) void WINAPI d_midiInGetDevCapsA() { _asm { jmp p_midiInGetDevCapsA } }
    __declspec(naked) void WINAPI d_midiInGetDevCapsW() { _asm { jmp p_midiInGetDevCapsW } }
    __declspec(naked) void WINAPI d_midiInGetErrorTextA() { _asm { jmp p_midiInGetErrorTextA } }
    __declspec(naked) void WINAPI d_midiInGetErrorTextW() { _asm { jmp p_midiInGetErrorTextW } }
    __declspec(naked) void WINAPI d_midiInGetID() { _asm { jmp p_midiInGetID } }
    __declspec(naked) void WINAPI d_midiInGetNumDevs() { _asm { jmp p_midiInGetNumDevs } }
    __declspec(naked) void WINAPI d_midiInMessage() { _asm { jmp p_midiInMessage } }
    __declspec(naked) void WINAPI d_midiInOpen() { _asm { jmp p_midiInOpen } }
    __declspec(naked) void WINAPI d_midiInPrepareHeader() { _asm { jmp p_midiInPrepareHeader } }
    __declspec(naked) void WINAPI d_midiInReset() { _asm { jmp p_midiInReset } }
    __declspec(naked) void WINAPI d_midiInStart() { _asm { jmp p_midiInStart } }
    __declspec(naked) void WINAPI d_midiInStop() { _asm { jmp p_midiInStop } }
    __declspec(naked) void WINAPI d_midiInUnprepareHeader() { _asm { jmp p_midiInUnprepareHeader } }
    __declspec(naked) void WINAPI d_midiOutCacheDrumPatches() { _asm { jmp p_midiOutCacheDrumPatches } }
    __declspec(naked) void WINAPI d_midiOutCachePatches() { _asm { jmp p_midiOutCachePatches } }
    __declspec(naked) void WINAPI d_midiOutClose() { _asm { jmp p_midiOutClose } }
    __declspec(naked) void WINAPI d_midiOutGetDevCapsA() { _asm { jmp p_midiOutGetDevCapsA } }
    __declspec(naked) void WINAPI d_midiOutGetDevCapsW() { _asm { jmp p_midiOutGetDevCapsW } }
    __declspec(naked) void WINAPI d_midiOutGetErrorTextA() { _asm { jmp p_midiOutGetErrorTextA } }
    __declspec(naked) void WINAPI d_midiOutGetErrorTextW() { _asm { jmp p_midiOutGetErrorTextW } }
    __declspec(naked) void WINAPI d_midiOutGetID() { _asm { jmp p_midiOutGetID } }
    __declspec(naked) void WINAPI d_midiOutGetNumDevs() { _asm { jmp p_midiOutGetNumDevs } }
    __declspec(naked) void WINAPI d_midiOutGetVolume() { _asm { jmp p_midiOutGetVolume } }
    __declspec(naked) void WINAPI d_midiOutLongMsg() { _asm { jmp p_midiOutLongMsg } }
    __declspec(naked) void WINAPI d_midiOutMessage() { _asm { jmp p_midiOutMessage } }
    __declspec(naked) void WINAPI d_midiOutOpen() { _asm { jmp p_midiOutOpen } }
    __declspec(naked) void WINAPI d_midiOutPrepareHeader() { _asm { jmp p_midiOutPrepareHeader } }
    __declspec(naked) void WINAPI d_midiOutReset() { _asm { jmp p_midiOutReset } }
    __declspec(naked) void WINAPI d_midiOutSetVolume() { _asm { jmp p_midiOutSetVolume } }
    __declspec(naked) void WINAPI d_midiOutShortMsg() { _asm { jmp p_midiOutShortMsg } }
    __declspec(naked) void WINAPI d_midiOutUnprepareHeader() { _asm { jmp p_midiOutUnprepareHeader } }
    __declspec(naked) void WINAPI d_midiStreamClose() { _asm { jmp p_midiStreamClose } }
    __declspec(naked) void WINAPI d_midiStreamOpen() { _asm { jmp p_midiStreamOpen } }
    __declspec(naked) void WINAPI d_midiStreamOut() { _asm { jmp p_midiStreamOut } }
    __declspec(naked) void WINAPI d_midiStreamPause() { _asm { jmp p_midiStreamPause } }
    __declspec(naked) void WINAPI d_midiStreamPosition() { _asm { jmp p_midiStreamPosition } }
    __declspec(naked) void WINAPI d_midiStreamProperty() { _asm { jmp p_midiStreamProperty } }
    __declspec(naked) void WINAPI d_midiStreamRestart() { _asm { jmp p_midiStreamRestart } }
    __declspec(naked) void WINAPI d_midiStreamStop() { _asm { jmp p_midiStreamStop } }
    __declspec(naked) void WINAPI d_mixerClose() { _asm { jmp p_mixerClose } }
    __declspec(naked) void WINAPI d_mixerGetControlDetailsA() { _asm { jmp p_mixerGetControlDetailsA } }
    __declspec(naked) void WINAPI d_mixerGetControlDetailsW() { _asm { jmp p_mixerGetControlDetailsW } }
    __declspec(naked) void WINAPI d_mixerGetDevCapsA() { _asm { jmp p_mixerGetDevCapsA } }
    __declspec(naked) void WINAPI d_mixerGetDevCapsW() { _asm { jmp p_mixerGetDevCapsW } }
    __declspec(naked) void WINAPI d_mixerGetID() { _asm { jmp p_mixerGetID } }
    __declspec(naked) void WINAPI d_mixerGetLineControlsA() { _asm { jmp p_mixerGetLineControlsA } }
    __declspec(naked) void WINAPI d_mixerGetLineControlsW() { _asm { jmp p_mixerGetLineControlsW } }
    __declspec(naked) void WINAPI d_mixerGetLineInfoA() { _asm { jmp p_mixerGetLineInfoA } }
    __declspec(naked) void WINAPI d_mixerGetLineInfoW() { _asm { jmp p_mixerGetLineInfoW } }
    __declspec(naked) void WINAPI d_mixerGetNumDevs() { _asm { jmp p_mixerGetNumDevs } }
    __declspec(naked) void WINAPI d_mixerMessage() { _asm { jmp p_mixerMessage } }
    __declspec(naked) void WINAPI d_mixerOpen() { _asm { jmp p_mixerOpen } }
    __declspec(naked) void WINAPI d_mixerSetControlDetails() { _asm { jmp p_mixerSetControlDetails } }
    __declspec(naked) void WINAPI d_mmDrvInstall() { _asm { jmp p_mmDrvInstall } }
    __declspec(naked) void WINAPI d_mmGetCurrentTask() { _asm { jmp p_mmGetCurrentTask } }
    __declspec(naked) void WINAPI d_mmTaskBlock() { _asm { jmp p_mmTaskBlock } }
    __declspec(naked) void WINAPI d_mmTaskCreate() { _asm { jmp p_mmTaskCreate } }
    __declspec(naked) void WINAPI d_mmTaskSignal() { _asm { jmp p_mmTaskSignal } }
    __declspec(naked) void WINAPI d_mmTaskYield() { _asm { jmp p_mmTaskYield } }
    __declspec(naked) void WINAPI d_mmioAdvance() { _asm { jmp p_mmioAdvance } }
    __declspec(naked) void WINAPI d_mmioAscend() { _asm { jmp p_mmioAscend } }
    __declspec(naked) void WINAPI d_mmioClose() { _asm { jmp p_mmioClose } }
    __declspec(naked) void WINAPI d_mmioCreateChunk() { _asm { jmp p_mmioCreateChunk } }
    __declspec(naked) void WINAPI d_mmioDescend() { _asm { jmp p_mmioDescend } }
    __declspec(naked) void WINAPI d_mmioFlush() { _asm { jmp p_mmioFlush } }
    __declspec(naked) void WINAPI d_mmioGetInfo() { _asm { jmp p_mmioGetInfo } }
    __declspec(naked) void WINAPI d_mmioInstallIOProcA() { _asm { jmp p_mmioInstallIOProcA } }
    __declspec(naked) void WINAPI d_mmioInstallIOProcW() { _asm { jmp p_mmioInstallIOProcW } }
    __declspec(naked) void WINAPI d_mmioOpenA() { _asm { jmp p_mmioOpenA } }
    __declspec(naked) void WINAPI d_mmioOpenW() { _asm { jmp p_mmioOpenW } }
    __declspec(naked) void WINAPI d_mmioRead() { _asm { jmp p_mmioRead } }
    __declspec(naked) void WINAPI d_mmioRenameA() { _asm { jmp p_mmioRenameA } }
    __declspec(naked) void WINAPI d_mmioRenameW() { _asm { jmp p_mmioRenameW } }
    __declspec(naked) void WINAPI d_mmioSeek() { _asm { jmp p_mmioSeek } }
    __declspec(naked) void WINAPI d_mmioSendMessage() { _asm { jmp p_mmioSendMessage } }
    __declspec(naked) void WINAPI d_mmioSetBuffer() { _asm { jmp p_mmioSetBuffer } }
    __declspec(naked) void WINAPI d_mmioSetInfo() { _asm { jmp p_mmioSetInfo } }
    __declspec(naked) void WINAPI d_mmioStringToFOURCCA() { _asm { jmp p_mmioStringToFOURCCA } }
    __declspec(naked) void WINAPI d_mmioStringToFOURCCW() { _asm { jmp p_mmioStringToFOURCCW } }
    __declspec(naked) void WINAPI d_mmioWrite() { _asm { jmp p_mmioWrite } }
    __declspec(naked) void WINAPI d_mmsystemGetVersion() { _asm { jmp p_mmsystemGetVersion } }
    __declspec(naked) void WINAPI d_mod32Message() { _asm { jmp p_mod32Message } }
    __declspec(naked) void WINAPI d_mxd32Message() { _asm { jmp p_mxd32Message } }
    __declspec(naked) void WINAPI d_sndPlaySoundA() { _asm { jmp p_sndPlaySoundA } }
    __declspec(naked) void WINAPI d_sndPlaySoundW() { _asm { jmp p_sndPlaySoundW } }
    __declspec(naked) void WINAPI d_tid32Message() { _asm { jmp p_tid32Message } }
    __declspec(naked) void WINAPI d_timeBeginPeriod() { _asm { jmp p_timeBeginPeriod } }
    __declspec(naked) void WINAPI d_timeEndPeriod() { _asm { jmp p_timeEndPeriod } }
    __declspec(naked) void WINAPI d_timeGetDevCaps() { _asm { jmp p_timeGetDevCaps } }
    __declspec(naked) void WINAPI d_timeGetSystemTime() { _asm { jmp p_timeGetSystemTime } }
    __declspec(naked) void WINAPI d_timeGetTime() { _asm { jmp p_timeGetTime } }
    __declspec(naked) void WINAPI d_timeKillEvent() { _asm { jmp p_timeKillEvent } }
    __declspec(naked) void WINAPI d_timeSetEvent() { _asm { jmp p_timeSetEvent } }
    __declspec(naked) void WINAPI d_waveInAddBuffer() { _asm { jmp p_waveInAddBuffer } }
    __declspec(naked) void WINAPI d_waveInClose() { _asm { jmp p_waveInClose } }
    __declspec(naked) void WINAPI d_waveInGetDevCapsA() { _asm { jmp p_waveInGetDevCapsA } }
    __declspec(naked) void WINAPI d_waveInGetDevCapsW() { _asm { jmp p_waveInGetDevCapsW } }
    __declspec(naked) void WINAPI d_waveInGetErrorTextA() { _asm { jmp p_waveInGetErrorTextA } }
    __declspec(naked) void WINAPI d_waveInGetErrorTextW() { _asm { jmp p_waveInGetErrorTextW } }
    __declspec(naked) void WINAPI d_waveInGetID() { _asm { jmp p_waveInGetID } }
    __declspec(naked) void WINAPI d_waveInGetNumDevs() { _asm { jmp p_waveInGetNumDevs } }
    __declspec(naked) void WINAPI d_waveInGetPosition() { _asm { jmp p_waveInGetPosition } }
    __declspec(naked) void WINAPI d_waveInMessage() { _asm { jmp p_waveInMessage } }
    __declspec(naked) void WINAPI d_waveInOpen() { _asm { jmp p_waveInOpen } }
    __declspec(naked) void WINAPI d_waveInPrepareHeader() { _asm { jmp p_waveInPrepareHeader } }
    __declspec(naked) void WINAPI d_waveInReset() { _asm { jmp p_waveInReset } }
    __declspec(naked) void WINAPI d_waveInStart() { _asm { jmp p_waveInStart } }
    __declspec(naked) void WINAPI d_waveInStop() { _asm { jmp p_waveInStop } }
    __declspec(naked) void WINAPI d_waveInUnprepareHeader() { _asm { jmp p_waveInUnprepareHeader } }
    __declspec(naked) void WINAPI d_waveOutBreakLoop() { _asm { jmp p_waveOutBreakLoop } }
    __declspec(naked) void WINAPI d_waveOutClose() { _asm { jmp p_waveOutClose } }
    __declspec(naked) void WINAPI d_waveOutGetDevCapsA() { _asm { jmp p_waveOutGetDevCapsA } }
    __declspec(naked) void WINAPI d_waveOutGetDevCapsW() { _asm { jmp p_waveOutGetDevCapsW } }
    __declspec(naked) void WINAPI d_waveOutGetErrorTextA() { _asm { jmp p_waveOutGetErrorTextA } }
    __declspec(naked) void WINAPI d_waveOutGetErrorTextW() { _asm { jmp p_waveOutGetErrorTextW } }
    __declspec(naked) void WINAPI d_waveOutGetID() { _asm { jmp p_waveOutGetID } }
    __declspec(naked) void WINAPI d_waveOutGetNumDevs() { _asm { jmp p_waveOutGetNumDevs } }
    __declspec(naked) void WINAPI d_waveOutGetPitch() { _asm { jmp p_waveOutGetPitch } }
    __declspec(naked) void WINAPI d_waveOutGetPlaybackRate() { _asm { jmp p_waveOutGetPlaybackRate } }
    __declspec(naked) void WINAPI d_waveOutGetPosition() { _asm { jmp p_waveOutGetPosition } }
    __declspec(naked) void WINAPI d_waveOutGetVolume() { _asm { jmp p_waveOutGetVolume } }
    __declspec(naked) void WINAPI d_waveOutMessage() { _asm { jmp p_waveOutMessage } }
    __declspec(naked) void WINAPI d_waveOutOpen() { _asm { jmp p_waveOutOpen } }
    __declspec(naked) void WINAPI d_waveOutPause() { _asm { jmp p_waveOutPause } }
    __declspec(naked) void WINAPI d_waveOutPrepareHeader() { _asm { jmp p_waveOutPrepareHeader } }
    __declspec(naked) void WINAPI d_waveOutReset() { _asm { jmp p_waveOutReset } }
    __declspec(naked) void WINAPI d_waveOutRestart() { _asm { jmp p_waveOutRestart } }
    __declspec(naked) void WINAPI d_waveOutSetPitch() { _asm { jmp p_waveOutSetPitch } }
    __declspec(naked) void WINAPI d_waveOutSetPlaybackRate() { _asm { jmp p_waveOutSetPlaybackRate } }
    __declspec(naked) void WINAPI d_waveOutSetVolume() { _asm { jmp p_waveOutSetVolume } }
    __declspec(naked) void WINAPI d_waveOutUnprepareHeader() { _asm { jmp p_waveOutUnprepareHeader } }
    __declspec(naked) void WINAPI d_waveOutWrite() { _asm { jmp p_waveOutWrite } }
    __declspec(naked) void WINAPI d_wid32Message() { _asm { jmp p_wid32Message } }
    __declspec(naked) void WINAPI d_wod32Message() { _asm { jmp p_wod32Message } }
}


void setDllFuncAddress()
{
    p_NONAME1 = GetProcAddress(hOriginalDll, MAKEINTRESOURCE(2));
    p_mciExecute = GetProcAddress(hOriginalDll, "mciExecute");
    p_CloseDriver = GetProcAddress(hOriginalDll, "CloseDriver");
    p_DefDriverProc = GetProcAddress(hOriginalDll, "DefDriverProc");
    p_DriverCallback = GetProcAddress(hOriginalDll, "DriverCallback");
    p_DrvGetModuleHandle = GetProcAddress(hOriginalDll, "DrvGetModuleHandle");
    p_GetDriverModuleHandle = GetProcAddress(hOriginalDll, "GetDriverModuleHandle");
    p_NotifyCallbackData = GetProcAddress(hOriginalDll, "NotifyCallbackData");
    p_OpenDriver = GetProcAddress(hOriginalDll, "OpenDriver");
    p_PlaySound = GetProcAddress(hOriginalDll, "PlaySound");
    p_PlaySoundA = GetProcAddress(hOriginalDll, "PlaySoundA");
    p_PlaySoundW = GetProcAddress(hOriginalDll, "PlaySoundW");
    p_SendDriverMessage = GetProcAddress(hOriginalDll, "SendDriverMessage");
    p_WOW32DriverCallback = GetProcAddress(hOriginalDll, "WOW32DriverCallback");
    p_WOW32ResolveMultiMediaHandle = GetProcAddress(hOriginalDll, "WOW32ResolveMultiMediaHandle");
    p_WOWAppExit = GetProcAddress(hOriginalDll, "WOWAppExit");
    p_aux32Message = GetProcAddress(hOriginalDll, "aux32Message");
    p_auxGetDevCapsA = GetProcAddress(hOriginalDll, "auxGetDevCapsA");
    p_auxGetDevCapsW = GetProcAddress(hOriginalDll, "auxGetDevCapsW");
    p_auxGetNumDevs = GetProcAddress(hOriginalDll, "auxGetNumDevs");
    p_auxGetVolume = GetProcAddress(hOriginalDll, "auxGetVolume");
    p_auxOutMessage = GetProcAddress(hOriginalDll, "auxOutMessage");
    p_auxSetVolume = GetProcAddress(hOriginalDll, "auxSetVolume");
    p_joy32Message = GetProcAddress(hOriginalDll, "joy32Message");
    p_joyConfigChanged = GetProcAddress(hOriginalDll, "joyConfigChanged");
    p_joyGetDevCapsA = GetProcAddress(hOriginalDll, "joyGetDevCapsA");
    p_joyGetDevCapsW = GetProcAddress(hOriginalDll, "joyGetDevCapsW");
    p_joyGetNumDevs = GetProcAddress(hOriginalDll, "joyGetNumDevs");
    p_joyGetPos = GetProcAddress(hOriginalDll, "joyGetPos");
    p_joyGetPosEx = GetProcAddress(hOriginalDll, "joyGetPosEx");
    p_joyGetThreshold = GetProcAddress(hOriginalDll, "joyGetThreshold");
    p_joyReleaseCapture = GetProcAddress(hOriginalDll, "joyReleaseCapture");
    p_joySetCapture = GetProcAddress(hOriginalDll, "joySetCapture");
    p_joySetThreshold = GetProcAddress(hOriginalDll, "joySetThreshold");
    p_mci32Message = GetProcAddress(hOriginalDll, "mci32Message");
    p_mciDriverNotify = GetProcAddress(hOriginalDll, "mciDriverNotify");
    p_mciDriverYield = GetProcAddress(hOriginalDll, "mciDriverYield");
    p_mciFreeCommandResource = GetProcAddress(hOriginalDll, "mciFreeCommandResource");
    p_mciGetCreatorTask = GetProcAddress(hOriginalDll, "mciGetCreatorTask");
    p_mciGetDeviceIDA = GetProcAddress(hOriginalDll, "mciGetDeviceIDA");
    p_mciGetDeviceIDFromElementIDA = GetProcAddress(hOriginalDll, "mciGetDeviceIDFromElementIDA");
    p_mciGetDeviceIDFromElementIDW = GetProcAddress(hOriginalDll, "mciGetDeviceIDFromElementIDW");
    p_mciGetDeviceIDW = GetProcAddress(hOriginalDll, "mciGetDeviceIDW");
    p_mciGetDriverData = GetProcAddress(hOriginalDll, "mciGetDriverData");
    p_mciGetErrorStringA = GetProcAddress(hOriginalDll, "mciGetErrorStringA");
    p_mciGetErrorStringW = GetProcAddress(hOriginalDll, "mciGetErrorStringW");
    p_mciGetYieldProc = GetProcAddress(hOriginalDll, "mciGetYieldProc");
    p_mciLoadCommandResource = GetProcAddress(hOriginalDll, "mciLoadCommandResource");
    p_mciSendCommandA = GetProcAddress(hOriginalDll, "mciSendCommandA");
    p_mciSendCommandW = GetProcAddress(hOriginalDll, "mciSendCommandW");
    p_mciSendStringA = GetProcAddress(hOriginalDll, "mciSendStringA");
    p_mciSendStringW = GetProcAddress(hOriginalDll, "mciSendStringW");
    p_mciSetDriverData = GetProcAddress(hOriginalDll, "mciSetDriverData");
    p_mciSetYieldProc = GetProcAddress(hOriginalDll, "mciSetYieldProc");
    p_mid32Message = GetProcAddress(hOriginalDll, "mid32Message");
    p_midiConnect = GetProcAddress(hOriginalDll, "midiConnect");
    p_midiDisconnect = GetProcAddress(hOriginalDll, "midiDisconnect");
    p_midiInAddBuffer = GetProcAddress(hOriginalDll, "midiInAddBuffer");
    p_midiInClose = GetProcAddress(hOriginalDll, "midiInClose");
    p_midiInGetDevCapsA = GetProcAddress(hOriginalDll, "midiInGetDevCapsA");
    p_midiInGetDevCapsW = GetProcAddress(hOriginalDll, "midiInGetDevCapsW");
    p_midiInGetErrorTextA = GetProcAddress(hOriginalDll, "midiInGetErrorTextA");
    p_midiInGetErrorTextW = GetProcAddress(hOriginalDll, "midiInGetErrorTextW");
    p_midiInGetID = GetProcAddress(hOriginalDll, "midiInGetID");
    p_midiInGetNumDevs = GetProcAddress(hOriginalDll, "midiInGetNumDevs");
    p_midiInMessage = GetProcAddress(hOriginalDll, "midiInMessage");
    p_midiInOpen = GetProcAddress(hOriginalDll, "midiInOpen");
    p_midiInPrepareHeader = GetProcAddress(hOriginalDll, "midiInPrepareHeader");
    p_midiInReset = GetProcAddress(hOriginalDll, "midiInReset");
    p_midiInStart = GetProcAddress(hOriginalDll, "midiInStart");
    p_midiInStop = GetProcAddress(hOriginalDll, "midiInStop");
    p_midiInUnprepareHeader = GetProcAddress(hOriginalDll, "midiInUnprepareHeader");
    p_midiOutCacheDrumPatches = GetProcAddress(hOriginalDll, "midiOutCacheDrumPatches");
    p_midiOutCachePatches = GetProcAddress(hOriginalDll, "midiOutCachePatches");
    p_midiOutClose = GetProcAddress(hOriginalDll, "midiOutClose");
    p_midiOutGetDevCapsA = GetProcAddress(hOriginalDll, "midiOutGetDevCapsA");
    p_midiOutGetDevCapsW = GetProcAddress(hOriginalDll, "midiOutGetDevCapsW");
    p_midiOutGetErrorTextA = GetProcAddress(hOriginalDll, "midiOutGetErrorTextA");
    p_midiOutGetErrorTextW = GetProcAddress(hOriginalDll, "midiOutGetErrorTextW");
    p_midiOutGetID = GetProcAddress(hOriginalDll, "midiOutGetID");
    p_midiOutGetNumDevs = GetProcAddress(hOriginalDll, "midiOutGetNumDevs");
    p_midiOutGetVolume = GetProcAddress(hOriginalDll, "midiOutGetVolume");
    p_midiOutLongMsg = GetProcAddress(hOriginalDll, "midiOutLongMsg");
    p_midiOutMessage = GetProcAddress(hOriginalDll, "midiOutMessage");
    p_midiOutOpen = GetProcAddress(hOriginalDll, "midiOutOpen");
    p_midiOutPrepareHeader = GetProcAddress(hOriginalDll, "midiOutPrepareHeader");
    p_midiOutReset = GetProcAddress(hOriginalDll, "midiOutReset");
    p_midiOutSetVolume = GetProcAddress(hOriginalDll, "midiOutSetVolume");
    p_midiOutShortMsg = GetProcAddress(hOriginalDll, "midiOutShortMsg");
    p_midiOutUnprepareHeader = GetProcAddress(hOriginalDll, "midiOutUnprepareHeader");
    p_midiStreamClose = GetProcAddress(hOriginalDll, "midiStreamClose");
    p_midiStreamOpen = GetProcAddress(hOriginalDll, "midiStreamOpen");
    p_midiStreamOut = GetProcAddress(hOriginalDll, "midiStreamOut");
    p_midiStreamPause = GetProcAddress(hOriginalDll, "midiStreamPause");
    p_midiStreamPosition = GetProcAddress(hOriginalDll, "midiStreamPosition");
    p_midiStreamProperty = GetProcAddress(hOriginalDll, "midiStreamProperty");
    p_midiStreamRestart = GetProcAddress(hOriginalDll, "midiStreamRestart");
    p_midiStreamStop = GetProcAddress(hOriginalDll, "midiStreamStop");
    p_mixerClose = GetProcAddress(hOriginalDll, "mixerClose");
    p_mixerGetControlDetailsA = GetProcAddress(hOriginalDll, "mixerGetControlDetailsA");
    p_mixerGetControlDetailsW = GetProcAddress(hOriginalDll, "mixerGetControlDetailsW");
    p_mixerGetDevCapsA = GetProcAddress(hOriginalDll, "mixerGetDevCapsA");
    p_mixerGetDevCapsW = GetProcAddress(hOriginalDll, "mixerGetDevCapsW");
    p_mixerGetID = GetProcAddress(hOriginalDll, "mixerGetID");
    p_mixerGetLineControlsA = GetProcAddress(hOriginalDll, "mixerGetLineControlsA");
    p_mixerGetLineControlsW = GetProcAddress(hOriginalDll, "mixerGetLineControlsW");
    p_mixerGetLineInfoA = GetProcAddress(hOriginalDll, "mixerGetLineInfoA");
    p_mixerGetLineInfoW = GetProcAddress(hOriginalDll, "mixerGetLineInfoW");
    p_mixerGetNumDevs = GetProcAddress(hOriginalDll, "mixerGetNumDevs");
    p_mixerMessage = GetProcAddress(hOriginalDll, "mixerMessage");
    p_mixerOpen = GetProcAddress(hOriginalDll, "mixerOpen");
    p_mixerSetControlDetails = GetProcAddress(hOriginalDll, "mixerSetControlDetails");
    p_mmDrvInstall = GetProcAddress(hOriginalDll, "mmDrvInstall");
    p_mmGetCurrentTask = GetProcAddress(hOriginalDll, "mmGetCurrentTask");
    p_mmTaskBlock = GetProcAddress(hOriginalDll, "mmTaskBlock");
    p_mmTaskCreate = GetProcAddress(hOriginalDll, "mmTaskCreate");
    p_mmTaskSignal = GetProcAddress(hOriginalDll, "mmTaskSignal");
    p_mmTaskYield = GetProcAddress(hOriginalDll, "mmTaskYield");
    p_mmioAdvance = GetProcAddress(hOriginalDll, "mmioAdvance");
    p_mmioAscend = GetProcAddress(hOriginalDll, "mmioAscend");
    p_mmioClose = GetProcAddress(hOriginalDll, "mmioClose");
    p_mmioCreateChunk = GetProcAddress(hOriginalDll, "mmioCreateChunk");
    p_mmioDescend = GetProcAddress(hOriginalDll, "mmioDescend");
    p_mmioFlush = GetProcAddress(hOriginalDll, "mmioFlush");
    p_mmioGetInfo = GetProcAddress(hOriginalDll, "mmioGetInfo");
    p_mmioInstallIOProcA = GetProcAddress(hOriginalDll, "mmioInstallIOProcA");
    p_mmioInstallIOProcW = GetProcAddress(hOriginalDll, "mmioInstallIOProcW");
    p_mmioOpenA =GetProcAddress(hOriginalDll, "mmioOpenA");
    p_mmioOpenW = GetProcAddress(hOriginalDll, "mmioOpenW");
    p_mmioRead = GetProcAddress(hOriginalDll, "mmioRead");
    p_mmioRenameA = GetProcAddress(hOriginalDll, "mmioRenameA");
    p_mmioRenameW = GetProcAddress(hOriginalDll, "mmioRenameW");
    p_mmioSeek = GetProcAddress(hOriginalDll, "mmioSeek");
    p_mmioSendMessage = GetProcAddress(hOriginalDll, "mmioSendMessage");
    p_mmioSetBuffer = GetProcAddress(hOriginalDll, "mmioSetBuffer");
    p_mmioSetInfo = GetProcAddress(hOriginalDll, "mmioSetInfo");
    p_mmioStringToFOURCCA = GetProcAddress(hOriginalDll, "mmioStringToFOURCCA");
    p_mmioStringToFOURCCW = GetProcAddress(hOriginalDll, "mmioStringToFOURCCW");
    p_mmioWrite = GetProcAddress(hOriginalDll, "mmioWrite");
    p_mmsystemGetVersion = GetProcAddress(hOriginalDll, "mmsystemGetVersion");
    p_mod32Message = GetProcAddress(hOriginalDll, "mod32Message");
    p_mxd32Message = GetProcAddress(hOriginalDll, "mxd32Message");
    p_sndPlaySoundA = GetProcAddress(hOriginalDll, "sndPlaySoundA");
    p_sndPlaySoundW = GetProcAddress(hOriginalDll, "sndPlaySoundW");
    p_tid32Message = GetProcAddress(hOriginalDll, "tid32Message");
    p_timeBeginPeriod = GetProcAddress(hOriginalDll, "timeBeginPeriod");
    p_timeEndPeriod = GetProcAddress(hOriginalDll, "timeEndPeriod");
    p_timeGetDevCaps = GetProcAddress(hOriginalDll, "timeGetDevCaps");
    p_timeGetSystemTime = GetProcAddress(hOriginalDll, "timeGetSystemTime");
    p_timeGetTime = GetProcAddress(hOriginalDll, "timeGetTime");
    p_timeKillEvent = GetProcAddress(hOriginalDll, "timeKillEvent");
    p_timeSetEvent = GetProcAddress(hOriginalDll, "timeSetEvent");
    p_waveInAddBuffer = GetProcAddress(hOriginalDll, "waveInAddBuffer");
    p_waveInClose = GetProcAddress(hOriginalDll, "waveInClose");
    p_waveInGetDevCapsA = GetProcAddress(hOriginalDll, "waveInGetDevCapsA");
    p_waveInGetDevCapsW = GetProcAddress(hOriginalDll, "waveInGetDevCapsW");
    p_waveInGetErrorTextA = GetProcAddress(hOriginalDll, "waveInGetErrorTextA");
    p_waveInGetErrorTextW = GetProcAddress(hOriginalDll, "waveInGetErrorTextW");
    p_waveInGetID = GetProcAddress(hOriginalDll, "waveInGetID");
    p_waveInGetNumDevs = GetProcAddress(hOriginalDll, "waveInGetNumDevs");
    p_waveInGetPosition = GetProcAddress(hOriginalDll, "waveInGetPosition");
    p_waveInMessage = GetProcAddress(hOriginalDll, "waveInMessage");
    p_waveInOpen = GetProcAddress(hOriginalDll, "waveInOpen");
    p_waveInPrepareHeader = GetProcAddress(hOriginalDll, "waveInPrepareHeader");
    p_waveInReset = GetProcAddress(hOriginalDll, "waveInReset");
    p_waveInStart = GetProcAddress(hOriginalDll, "waveInStart");
    p_waveInStop = GetProcAddress(hOriginalDll, "waveInStop");
    p_waveInUnprepareHeader = GetProcAddress(hOriginalDll, "waveInUnprepareHeader");
    p_waveOutBreakLoop = GetProcAddress(hOriginalDll, "waveOutBreakLoop");
    p_waveOutClose = GetProcAddress(hOriginalDll, "waveOutClose");
    p_waveOutGetDevCapsA = GetProcAddress(hOriginalDll, "waveOutGetDevCapsA");
    p_waveOutGetDevCapsW = GetProcAddress(hOriginalDll, "waveOutGetDevCapsW");
    p_waveOutGetErrorTextA = GetProcAddress(hOriginalDll, "waveOutGetErrorTextA");
    p_waveOutGetErrorTextW = GetProcAddress(hOriginalDll, "waveOutGetErrorTextW");
    p_waveOutGetID = GetProcAddress(hOriginalDll, "waveOutGetID");
    p_waveOutGetNumDevs = GetProcAddress(hOriginalDll, "waveOutGetNumDevs");
    p_waveOutGetPitch = GetProcAddress(hOriginalDll, "waveOutGetPitch");
    p_waveOutGetPlaybackRate = GetProcAddress(hOriginalDll, "waveOutGetPlaybackRate");
    p_waveOutGetPosition = GetProcAddress(hOriginalDll, "waveOutGetPosition");
    p_waveOutGetVolume = GetProcAddress(hOriginalDll, "waveOutGetVolume");
    p_waveOutMessage = GetProcAddress(hOriginalDll, "waveOutMessage");
    p_waveOutOpen = GetProcAddress(hOriginalDll, "waveOutOpen");
    p_waveOutPause = GetProcAddress(hOriginalDll, "waveOutPause");
    p_waveOutPrepareHeader = GetProcAddress(hOriginalDll, "waveOutPrepareHeader");
    p_waveOutReset = GetProcAddress(hOriginalDll, "waveOutReset");
    p_waveOutRestart = GetProcAddress(hOriginalDll, "waveOutRestart");
    p_waveOutSetPitch = GetProcAddress(hOriginalDll, "waveOutSetPitch");
    p_waveOutSetPlaybackRate = GetProcAddress(hOriginalDll, "waveOutSetPlaybackRate");
    p_waveOutSetVolume = GetProcAddress(hOriginalDll, "waveOutSetVolume");
    p_waveOutUnprepareHeader = GetProcAddress(hOriginalDll, "waveOutUnprepareHeader");
    p_waveOutWrite = GetProcAddress(hOriginalDll, "waveOutWrite");
    p_wid32Message = GetProcAddress(hOriginalDll, "wid32Message");
    p_wod32Message = GetProcAddress(hOriginalDll, "wod32Message");
}