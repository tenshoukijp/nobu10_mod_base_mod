#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>

#include "output_debug_stream.h"
#include "game_font.h"
#include "game_process.h"
#include "game_window.h"
// #include "onigwrap.h"
// #include "on_event.h"
// #include "hook_textouta_custom.h"


// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")


// ひとつのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInOneMod(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew,
    HMODULE hmodCaller)
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while (pImportDesc->Name) {
        PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0)
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
        ((PBYTE)hmodCaller + pImportDesc->FirstThunk);

    while (pThunk->u1.Function) {
        PROC* ppfn = (PROC*)&pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                hCurrentProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// すべてのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInAllMods(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew)
{
    // モジュールリストを取得
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // それぞれのモジュールに対してReplaceIATEntryInOneModを実行
    while (bModuleResult) {
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}


//---------------------------WindowProcA

// フックする関数のプロトタイプを定義
using PFNDEFWINDOWPROCA = LRESULT(WINAPI *)(HWND, UINT, WPARAM, LPARAM);

PROC pfnOrigDefWindowProcA = GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");

extern LRESULT Hook_DefWindowProcACustom(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Hook_DefWindowProcA(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
    Hook_DefWindowProcACustom(hWnd, Msg, wParam, lParam);

	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
	LRESULT LPResult = ((PFNDEFWINDOWPROCA)pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam);

	return LPResult;
}


/**/
//---------------------------TextOutA

// フックする関数のプロトタイプを定義
using PFNTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, LPCTSTR, int);

PROC pfnOrigTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");

extern BOOL isOverrideTextOut;

BOOL WINAPI Hook_TextOutA(
    HDC hdc,           // デバイスコンテキストのハンドル
    int nXStart,       // 開始位置（基準点）の x 座標
    int nYStart,       // 開始位置（基準点）の y 座標
    LPCTSTR lpString,  // 文字列
    int cbString       // 文字数
) {
    // 先にカスタムの方を実行。
    BOOL nResult = ((PFNTEXTOUTA)pfnOrigTextOutA)(hdc, nXStart, nYStart, lpString, cbString);

    return nResult;
}


//---------------------------CreateFontA
using PFNCREATEFONTA = HFONT (WINAPI *)(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR);

PROC pfnOrigCreateFontA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");

HFONT WINAPI Hook_CreateFontA(
    int    cHeight,
    int    cWidth,
    int    cEscapement,
    int    cOrientation,
    int    cWeight,
    DWORD  bItalic,
    DWORD  bUnderline,
    DWORD  bStrikeOut,
    DWORD  iCharSet,
    DWORD  iOutPrecision,
    DWORD  iClipPrecision,
    DWORD  iQuality,
    DWORD  iPitchAndFamily,
    LPCSTR pszFaceName
) {
    HFONT hFont = NULL;
    // ＭＳ 明朝 なら 将星 明朝へ
    if (std::string(pszFaceName) == "ＭＳ 明朝") {
        std::string strOverrideFontName = getNB8FontName();
        if (std::string(strOverrideFontName) != "") {
            hFont = ((PFNCREATEFONTA)pfnOrigCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, strOverrideFontName.c_str());
			return hFont;
        }
    }

    hFont = ((PFNCREATEFONTA)pfnOrigCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
    // フォントファミリーを指定のもので上書きする
    return hFont;
}

//---------------------------SetMenu
using PFNSETMENU = BOOL(WINAPI *)(HWND, HMENU);



//---------------------------ReleaseDC

using PFNRELEASEDC = int(WINAPI *)(HWND, HDC);

PROC pfnOrigReleaseDC = GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");

int WINAPI Hook_ReleaseDC(
    HWND hWnd,  // ウィンドウのハンドル
    HDC hDC     // デバイスコンテキストのハンドル
) {

	// 元のものを呼び出す
	int nResult = ((PFNRELEASEDC)pfnOrigReleaseDC)(hWnd, hDC);

	return nResult;
}


//---------------------------IsDebuggerPresent

using PFNISDEBUGGERPRESENT = BOOL(WINAPI *)();

PROC pfnOrigIsDebuggerPresent = GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");

BOOL WINAPI Hook_IsDebuggerPresent() {

	// 元のもの
	BOOL nResult = ((PFNISDEBUGGERPRESENT)pfnOrigIsDebuggerPresent)();

	return FALSE;
}




/*----------------------------------------------------------------*
 HOOK系処理
 *----------------------------------------------------------------*/
bool isHookDefWindowProcA = false;
bool isHookTextOutA = false;
bool isHookCreateFontA = false;
bool isHookSetMenu = false;
bool isHookReleaseDC = false;
bool isHookEnableMenuItem = false;
bool isHookBitBlt = false;
bool isHookCreateDIBitmap = false;
bool isHookCreateCompatibleDC = false;
bool isHookGetDIBits = false;
bool isHookIsDebuggerPresent = false;
bool isHookUnknown1 = false;

void hookFunctionsReplace() {

    PROC pfnOrig;
    if (!isHookDefWindowProcA) {
        isHookDefWindowProcA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
    }
    if (!isHookTextOutA) {
        isHookTextOutA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");
        ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_TextOutA);
    }
    if (!isHookCreateFontA) {
		isHookCreateFontA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");
		ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_CreateFontA);
    }
    if (!isHookReleaseDC) {
		isHookReleaseDC = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");
		ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_ReleaseDC);
	}
    if (!isHookIsDebuggerPresent) {
		isHookIsDebuggerPresent = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_IsDebuggerPresent);
	}
}