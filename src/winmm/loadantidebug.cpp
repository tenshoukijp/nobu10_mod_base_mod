#include <windows.h>
#include "output_debug_stream.h"

void loadAntiDebug() {
	// Steamのアンチデバッグをさらにアンチし、ブレイクポイント系が潰されるのをかなりの範囲で取り消す
	HMODULE hModule = LoadLibraryA("AntiDebug.mod.dll");
	if (hModule) {
		OutputDebugStream("AntiDebug.mod.dllのロード");
	}
}