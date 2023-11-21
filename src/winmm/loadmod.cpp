#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
using PFNONFINALIZE = void (WINAPI*)();

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

// 将星録Mod.dllの読み込み
HMODULE hNB10ModDll = NULL;
BOOL loadNB10Mod() {

	hNB10ModDll = LoadLibrary("蒼天録.mod.dll");

	// 存在しない
	if (!hNB10ModDll) {
		MessageBoxA(NULL, "蒼天録Modが無い", "エラー", NULL);
		return FALSE;
	}

	return TRUE;
}

void freeNB10Mod() {
	if (hNB10ModDll) {
		FreeLibrary(hNB10ModDll);
		hNB10ModDll = NULL;
	}
}

HWND hNB10Wnd = NULL;
void Mod_onInitialize() {
	pMod_onInitialize = (PFNONINITIALIZE)GetProcAddress(hNB10ModDll, "onInitialize");
	if (hNB10ModDll && pMod_onInitialize) {
		pMod_onInitialize(&hNB10Wnd);
	}
}

