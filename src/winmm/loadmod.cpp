#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
using PFNONFINALIZE = void (WINAPI*)();
using PFNONMMIOOPENA = void (WINAPI*)(char*, char*);

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;
PFNONMMIOOPENA  pMod_onMmioOpenA = NULL;

// è´êØò^Mod.dllÇÃì«Ç›çûÇ›
HMODULE hNB10ModDll = NULL;
BOOL loadNB10Mod() {

	hNB10ModDll = LoadLibrary("ëììVò^.mod.dll");

	// ë∂ç›ÇµÇ»Ç¢
	if (!hNB10ModDll) {
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

void Mod_onMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	pMod_onMmioOpenA = (PFNONMMIOOPENA)GetProcAddress(hNB10ModDll, "onMmioOpenA");
	if (hNB10ModDll && pMod_onMmioOpenA) {
		pMod_onMmioOpenA(pszFileName, bufOverrideFileName);
	}
}




