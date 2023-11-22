#include <windows.h>
#include "game_font.h"
#include "javascript_mod.h"
#include "output_debug_stream.h"

HWND hNB10Wnd = NULL;

HWND* referenceOfNB10Wnd = NULL;


void onCreateWindow(HWND hWnd) {


	// ウィンドウハンドルを保存
	hNB10Wnd = hWnd;

	// 呼び出し元(WinMM.dll)に、「今回の起動ではちゃんとウィンドウが生成されたよ」と伝える。よってゲーム本体が起動された。
	*referenceOfNB10Wnd = hWnd;

	// JavaScriptMod
	callJSModCreateWindow(hWnd);

	OutputDebugStream("ウィンドウ生成\n");

	OutputDebugStream("蒼天録の開始\n");
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		// JavaScriptMod
		callJSModDestroyWindow();

		OutputDebugStream("ウィンドウ破棄\r\n");
	}

	doneDestroyWindow = TRUE;
}
