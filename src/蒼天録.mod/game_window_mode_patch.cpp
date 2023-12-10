#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
#include "ddraw.h"

using namespace std;

#pragma unmanaged

/*
007CEF2E   83C4 28          ADD ESP,28
007CEF31   6A 13            PUSH 13                               Ç±ÇÃ13 ÇÕ DDSCL_FULLSCREEN | DDSCL_NOWINDOWCHANGES | DDSCL_EXCLUSIVE Ç∆ÇΩÇøÇ™à´Ç¢ÅB8Ç™ó«Ç¢
007CEF33   52               PUSH EDX
*/
void WriteAsmJumperOnSSRExeChangeDisplayMode() {

	BYTE *mode = (BYTE *)0x7CEF32; 
	// *mode = 0x13;
	// char sz[2] = { DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE, 0x00 };
	char sz[2] = { DDSCL_NORMAL, 0x00 };

	WriteProcessMemory(hCurrentProcess, (LPVOID)(0x7CEF32), sz, 1, NULL);

}

#pragma managed
