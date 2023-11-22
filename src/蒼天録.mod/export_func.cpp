#include <windows.h>
#include <winuser.h>
#include <stdio.h>

#include "game_process.h"
#include "game_window.h"
#include "game_font.h"

#include "hook_functions_replace.h"
#include "javascript_mod.h"
// #include "hook_functions_direct.h"

#include "output_debug_stream.h"
#include <time.h>

#pragma comment(lib, "user32.lib")

// WinMM.dllから呼ばれる関数。概ね初期時に呼ばれる。
extern "C" __declspec(dllexport) void WINAPI onInitialize(void* bufOfNB10Wnd) {

	srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化

	// 本当に将星録本体のゲームが始まり、ウィンドウが描画されたのかのチェック。同じEXEでもランチャーへとバイパスされたりなど、色々あるためこのチェックが欠かせない。
	referenceOfNB10Wnd = (HWND *)bufOfNB10Wnd;

	hookFunctionsReplace();

	char buffer[256] = "";
	sprintf_s(buffer, _countof(buffer), "%x", nBaseAddress);
	OutputDebugStream(buffer);

	OutputDebugStream("onInitialize\r\n");
}

extern "C" __declspec(dllexport) void WINAPI onMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	callJSModMmioOpenA(pszFileName, bufOverrideFileName);
}