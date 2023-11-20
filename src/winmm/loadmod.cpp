#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
extern PFNONINITIALIZE pMod_onInitialize;

using PFNONGAMEINITIALIZE = void (WINAPI*)();
extern PFNONGAMEINITIALIZE pMod_onGameInitialize;

using PFNONFINALIZE = void (WINAPI*)();
extern PFNONFINALIZE pMod_onFinalize;

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONGAMEINITIALIZE pMod_onGameInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

// è´êØò^Mod.dllÇÃì«Ç›çûÇ›
HMODULE hNB10ModDll = NULL;
BOOL loadNB10Mod() {

	hNB10ModDll = LoadLibrary("ëììVò^.mod.dll");

	// ë∂ç›ÇµÇ»Ç¢
	if (!hNB10ModDll) {
		MessageBoxA(NULL, "ëììVò^ModÇ™ñ≥Ç¢", "ÉGÉâÅ[", NULL);
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

