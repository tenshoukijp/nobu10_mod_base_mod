#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "output_debug_stream.h"
#include "game_font.h"
#include "game_process.h"
#include "game_window.h"
// #include "onigwrap.h"
// #include "on_event.h"
// #include "hook_textouta_custom.h"

#include "javascript_mod.h"
#include "file_attribute.h"

// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")

using namespace std;

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


//---------------------------TextOutA

using PFNTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, LPCTSTR, int);

PROC pfnOrigTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");

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


//---------------------------ExtTextOutA

using PFNEXTTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, UINT, const RECT *, LPCSTR, UINT, const INT *);

PROC pfnOrigExtTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "ExtTextOutA");

/*
* 1箇所目(2箇所目のすぐ上)
007E2F90  |. 6A 00          |PUSH 0                                  ; /pSpacing = NULL
007E2F92  |. 56             |PUSH ESI                                ; |StringSize
007E2F93  |. 51             |PUSH ECX                                ; |String
007E2F94  |. 8B0F           |MOV ECX,DWORD PTR DS:[EDI]              ; |
007E2F96  |. 6A 00          |PUSH 0                                  ; |pRect = NULL
007E2F98  |. 6A 00          |PUSH 0                                  ; |Options = 0
007E2F9A  |. 8D4410 01      |LEA EAX,DWORD PTR DS:[EAX+EDX+1]        ; |
007E2F9E  |. 8B5424 4C      |MOV EDX,DWORD PTR SS:[ESP+4C]           ; |
007E2FA2  |. 50             |PUSH EAX                                ; |Y
007E2FA3  |. 41             |INC ECX                                 ; |
007E2FA4  |. 51             |PUSH ECX                                ; |X
007E2FA5  |. 52             |PUSH EDX                                ; |hDC
007E2FA6  |. FF15 44508700  |CALL DWORD PTR DS:[<&GDI32.ExtTextOutA>>; \ExtTextOutA

*　2箇所目 (1箇所目のすぐ下)
007E2FCA  |. 6A 00          |PUSH 0                                  ; /pSpacing = NULL
007E2FCC  |. 56             |PUSH ESI                                ; |StringSize
007E2FCD  |. 8B77 04        |MOV ESI,DWORD PTR DS:[EDI+4]            ; |
007E2FD0  |. 52             |PUSH EDX                                ; |String
007E2FD1  |. 6A 00          |PUSH 0                                  ; |pRect = NULL
007E2FD3  |. 6A 00          |PUSH 0                                  ; |Options = 0
007E2FD5  |. 03C6           |ADD EAX,ESI                             ; |
007E2FD7  |. 50             |PUSH EAX                                ; |Y
007E2FD8  |. 8B07           |MOV EAX,DWORD PTR DS:[EDI]              ; |
007E2FDA  |. 50             |PUSH EAX                                ; |X
007E2FDB  |. 51             |PUSH ECX                                ; |hDC
007E2FDC  |. FF15 44508700  |CALL DWORD PTR DS:[<&GDI32.ExtTextOutA>>; \ExtTextOutA


*/
BOOL WINAPI Hook_ExtTextOutA(
	HDC hdc,           // デバイスコンテキストのハンドル
	int nXStart,       // 開始位置（基準点）の x 座標
	int nYStart,       // 開始位置（基準点）の y 座標
	UINT fuOptions,    // オプション
	const RECT *lprc,  // クリッピング矩形
	LPCSTR lpString,   // 文字列
	UINT cbCount,      // 文字数
	const INT *lpDx    // 文字間隔配列
) {
    // ここで直接操作する必要はなくなった。hook_textdraw.cppで操作
	// OutputDebugStream("ExtTextOutA:%s\n", lpString);
    // OutputDebugStream("ExtTextOutAdd:%x\n", lpString);
    // OutputDebugStream("関数の場所%x\n", Hook_ExtTextOutA);

	// 先にカスタムの方を実行。
	BOOL nResult = ((PFNEXTTEXTOUTA)pfnOrigExtTextOutA)(hdc, nXStart, nYStart, fuOptions, lprc, lpString, cbCount, lpDx);

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


//---------------------------CreateWindowExA

using PFNCREATEWINDOWEXA = HWND(WINAPI *)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);

PROC pfnOrigCreateWindowExA = GetProcAddress(GetModuleHandleA("user32.dll"), "CreateWindowExA");

HWND WINAPI Hook_CreateWindowExA(
    DWORD     dwExStyle,        // 拡張ウィンドウスタイル
    LPCSTR    lpClassName,      // ウィンドウクラス名
    LPCSTR    lpWindowName,     // ウィンドウ名
    DWORD     dwStyle,          // ウィンドウスタイル
    int       x,                // ウィンドウの左上隅の x 座標
    int       y,                // ウィンドウの左上隅の y 座標
    int       nWidth,           // ウィンドウの幅
    int       nHeight,          // ウィンドウの高さ
    HWND      hWndParent,       // 親ウィンドウのハンドル
    HMENU     hMenu,            // メニューのハンドル
    HINSTANCE hInstance,        // インスタンスのハンドル
    LPVOID    lpParam           // 作成データ
) {

    OutputDebugStream("幅%d", nWidth);
    OutputDebugStream("高%d", nHeight);
    OutputDebugStream("dwStyle%d", dwStyle);

    // ウィンドウ状態をなんとか保つ
    HWND hWnd = ((PFNCREATEWINDOWEXA)pfnOrigCreateWindowExA)(0, lpClassName, lpWindowName, 0, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    // 元のものを呼び出す
	// HWND hWnd = ((PFNCREATEWINDOWEXA)pfnOrigCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return hWnd;
}

//---------------------------SetWindowLongA

using PFNSETWINDOWLONGA = LONG(WINAPI *)(HWND, int, LONG);

PROC pfnOrigSetWindowLongA = GetProcAddress(GetModuleHandleA("user32.dll"), "SetWindowLongA");

int count = 0;
LONG WINAPI Hook_SetWindowLongA(
    HWND hWnd,      // ウィンドウのハンドル
    int nIndex,     // ウィンドウのプロパティ
    LONG dwNewLong  // 新しい値
) {
    OutputDebugStream("SetWindowLongAの呼び出し\n");
    if (nIndex == GWL_STYLE) {
        OutputDebugStream("GWL_STYLEの呼び出し\n");
    }
    if (nIndex == GWL_EXSTYLE) {
        OutputDebugStream("GWL_EXSTYLEの呼び出し\n");
    }
    else {
        OutputDebugStream("%xの呼び出し\n", nIndex);

    }
	// 元のものを呼び出す
	LONG nResult = ((PFNSETWINDOWLONGA)pfnOrigSetWindowLongA)(hWnd, nIndex, dwNewLong);

    // LONG nResult = ((PFNSETWINDOWLONGA)pfnOrigSetWindowLongA)(hWnd, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE);

	return nResult;
}

//---------------------------CreateFileA

using PFNCREATEFILEA = HANDLE(WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

PROC pfnOrigCreateFileA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
// extern HANDLE Hook_CreateFileACustom(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE WINAPI Hook_CreateFileA(
    LPCSTR lpFileName, // ファイル名
    DWORD dwDesiredAccess, // アクセス方法
    DWORD dwShareMode, // 共有方法
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
    DWORD dwCreationDisposition, // 作成方法
    DWORD dwFlagsAndAttributes, // ファイル属性
    HANDLE hTemplateFile // テンプレートファイルのハンドル
) {
    // 先にカスタムの方を実行。
    // Hook_CreateFileACustom(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    HANDLE nResult;

    // JS側からファイル名の変更要求があれば、ぞれ。
   // string jsOverrideFilePath = callJSModRequestFile(lpFileName);
    // デフォルトでチェックするオーバーライドファイル
    string dfOverrideFilePath = string("OVERRIDE\\") + lpFileName;
    /*
    if (jsOverrideFilePath.size() > 0) {
        OutputDebugStream("ファイル名を上書きします。%s\n", jsOverrideFilePath.c_str());
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(jsOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
    else*/ if (isFileExists(dfOverrideFilePath)) {
        // 元のもの
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(dfOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
    else {
        // 元のもの
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    // 元のもの
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
bool isHookExtTextOutA = false;
bool isHookCreateFontA = false;
bool isHookReleaseDC = false;
bool isHookCreateFileA = false;
bool isHookCreateWindowExA = false;
bool isHookSetWindowLongA = false;
bool isHookIsDebuggerPresent = false;

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
if (!isHookExtTextOutA) {
		isHookExtTextOutA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "ExtTextOutA");
		ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_ExtTextOutA);
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
    if (!isHookCreateFileA) {
        isHookCreateFileA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
        ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_CreateFileA);
    }
    if (!isHookCreateWindowExA) {
        isHookCreateWindowExA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "CreateWindowExA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_CreateWindowExA);
    }
    if (!isHookSetWindowLongA) {
		isHookSetWindowLongA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "SetWindowLongA");
		ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_SetWindowLongA);
	}
    if (!isHookIsDebuggerPresent) {
		isHookIsDebuggerPresent = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_IsDebuggerPresent);
	}
}