#include <windows.h>
#include "process.h"

FARPROC p_AcquireDDThreadLock;
FARPROC p_CompleteCreateSysmemSurface;
FARPROC p_D3DParseUnknownCommand;
FARPROC p_DDGetAttachedSurfaceLcl;
FARPROC p_DDInternalLock;
FARPROC p_DDInternalUnlock;
FARPROC p_DSoundHelp;
FARPROC p_DirectDrawCreate;
FARPROC p_DirectDrawCreateClipper;
FARPROC p_DirectDrawCreateEx;
FARPROC p_DirectDrawEnumerateA;
FARPROC p_DirectDrawEnumerateExA;
FARPROC p_DirectDrawEnumerateExW;
FARPROC p_DirectDrawEnumerateW;
FARPROC p_DllCanUnloadNow;
FARPROC p_DllGetClassObject;
FARPROC p_GetDDSurfaceLocal;
FARPROC p_GetOLEThunkData;
FARPROC p_GetSurfaceFromDC;
FARPROC p_RegisterSpecialCase;
FARPROC p_ReleaseDDThreadLock;
FARPROC p_SetAppCompatData;
extern "C" {
    __declspec(naked) void WINAPI d_AcquireDDThreadLock() { _asm { jmp p_AcquireDDThreadLock } }
    __declspec(naked) void WINAPI d_CompleteCreateSysmemSurface() { _asm { jmp p_CompleteCreateSysmemSurface } }
    __declspec(naked) void WINAPI d_D3DParseUnknownCommand() { _asm { jmp p_D3DParseUnknownCommand } }
    __declspec(naked) void WINAPI d_DDGetAttachedSurfaceLcl() { _asm { jmp p_DDGetAttachedSurfaceLcl } }
    __declspec(naked) void WINAPI d_DDInternalLock() { _asm { jmp p_DDInternalLock } }
    __declspec(naked) void WINAPI d_DDInternalUnlock() { _asm { jmp p_DDInternalUnlock } }
    __declspec(naked) void WINAPI d_DSoundHelp() { _asm { jmp p_DSoundHelp } }
    __declspec(naked) void WINAPI d_DirectDrawCreate() { _asm { jmp p_DirectDrawCreate } }
    __declspec(naked) void WINAPI d_DirectDrawCreateClipper() { _asm { jmp p_DirectDrawCreateClipper } }
    __declspec(naked) void WINAPI d_DirectDrawCreateEx() { _asm { jmp p_DirectDrawCreateEx } }
    __declspec(naked) void WINAPI d_DirectDrawEnumerateA() { _asm { jmp p_DirectDrawEnumerateA } }
    __declspec(naked) void WINAPI d_DirectDrawEnumerateExA() { _asm { jmp p_DirectDrawEnumerateExA } }
    __declspec(naked) void WINAPI d_DirectDrawEnumerateExW() { _asm { jmp p_DirectDrawEnumerateExW } }
    __declspec(naked) void WINAPI d_DirectDrawEnumerateW() { _asm { jmp p_DirectDrawEnumerateW } }
    __declspec(naked) void WINAPI d_DllCanUnloadNow() { _asm { jmp p_DllCanUnloadNow } }
    __declspec(naked) void WINAPI d_DllGetClassObject() { _asm { jmp p_DllGetClassObject } }
    __declspec(naked) void WINAPI d_GetDDSurfaceLocal() { _asm { jmp p_GetDDSurfaceLocal } }
    __declspec(naked) void WINAPI d_GetOLEThunkData() { _asm { jmp p_GetOLEThunkData } }
    __declspec(naked) void WINAPI d_GetSurfaceFromDC() { _asm { jmp p_GetSurfaceFromDC } }
    __declspec(naked) void WINAPI d_RegisterSpecialCase() { _asm { jmp p_RegisterSpecialCase } }
    __declspec(naked) void WINAPI d_ReleaseDDThreadLock() { _asm { jmp p_ReleaseDDThreadLock } }
    __declspec(naked) void WINAPI d_SetAppCompatData() { _asm { jmp p_SetAppCompatData } }
}

HINSTANCE hOriginalDll;
void setDllFuncAddress() {
    if (!hOriginalDll) {
        return;
    }

    p_AcquireDDThreadLock = GetProcAddress(hOriginalDll, "AcquireDDThreadLock");
    p_CompleteCreateSysmemSurface = GetProcAddress(hOriginalDll, "CompleteCreateSysmemSurface");
    p_D3DParseUnknownCommand = GetProcAddress(hOriginalDll, "D3DParseUnknownCommand");
    p_DDGetAttachedSurfaceLcl = GetProcAddress(hOriginalDll, "DDGetAttachedSurfaceLcl");
    p_DDInternalLock = GetProcAddress(hOriginalDll, "DDInternalLock");
    p_DDInternalUnlock = GetProcAddress(hOriginalDll, "DDInternalUnlock");
    p_DSoundHelp = GetProcAddress(hOriginalDll, "DSoundHelp");
    p_DirectDrawCreate = GetProcAddress(hOriginalDll, "DirectDrawCreate");
    p_DirectDrawCreateClipper = GetProcAddress(hOriginalDll, "DirectDrawCreateClipper");
    p_DirectDrawCreateEx = GetProcAddress(hOriginalDll, "DirectDrawCreateEx");
    p_DirectDrawEnumerateA = GetProcAddress(hOriginalDll, "DirectDrawEnumerateA");
    p_DirectDrawEnumerateExA = GetProcAddress(hOriginalDll, "DirectDrawEnumerateExA");
    p_DirectDrawEnumerateExW = GetProcAddress(hOriginalDll, "DirectDrawEnumerateExW");
    p_DirectDrawEnumerateW = GetProcAddress(hOriginalDll, "DirectDrawEnumerateW");
    p_DllCanUnloadNow = GetProcAddress(hOriginalDll, "DllCanUnloadNow");
    p_DllGetClassObject = GetProcAddress(hOriginalDll, "DllGetClassObject");
    p_GetDDSurfaceLocal = GetProcAddress(hOriginalDll, "GetDDSurfaceLocal");
    p_GetOLEThunkData = GetProcAddress(hOriginalDll, "GetOLEThunkData");
    p_GetSurfaceFromDC = GetProcAddress(hOriginalDll, "GetSurfaceFromDC");
    p_RegisterSpecialCase = GetProcAddress(hOriginalDll, "RegisterSpecialCase");
    p_ReleaseDDThreadLock = GetProcAddress(hOriginalDll, "ReleaseDDThreadLock");
    p_SetAppCompatData = GetProcAddress(hOriginalDll, "SetAppCompatData");
}


BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!IsWow64()) {
            MessageBox(NULL, "エラー", "「SysWow64」環境ではないようです。", NULL);
            return FALSE;
        }

        // C:\Windows\SysWow64のパスを取得。ほとんど全ての人はCドライブから変更していないとは思うが...
        char sysWow64Path[512] = "";
        UINT hasWow64Path = GetSystemWow64DirectoryA(sysWow64Path, sizeof(sysWow64Path));
        if (hasWow64Path == 0) {
            MessageBox(NULL, "エラー", "「SysWow64」のパスを取得できませんでした。", NULL);
            return FALSE;
        }
        strcat_s(sysWow64Path, "\\ddraw.dll");

        hOriginalDll = LoadLibrary(sysWow64Path);
        if (hOriginalDll == NULL) {
            MessageBox(NULL, "エラー", "「SysWow64」内のddraw.dllをロードできませんでした。", NULL);
            return FALSE;
        }

        setDllFuncAddress();

        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        FreeLibrary(hOriginalDll);
        break;
    default:
        break;
    }
    return TRUE;
}
