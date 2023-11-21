#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
using PFNONFINALIZE = void (WINAPI*)();

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

// �����^Mod.dll�̓ǂݍ���
HMODULE hNB10ModDll = NULL;
BOOL loadNB10Mod() {

	hNB10ModDll = LoadLibrary("���V�^.mod.dll");

	// ���݂��Ȃ�
	if (!hNB10ModDll) {
		MessageBoxA(NULL, "���V�^Mod������", "�G���[", NULL);
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

