#include <windows.h>
#include "game_font.h"
#include "output_debug_stream.h"

HWND hNB10Wnd = NULL;

HWND* referenceOfNB10Wnd = NULL;

// dllが破棄された時に、onDestroyWindowを実行するため。
extern void onDestroyWindow();
class GlobalDummyWindow {
	int isDummy;
public:
	GlobalDummyWindow() {
		isDummy = 0;
		OutputDebugStream("ダミーインスタンス生成\n");
	}
	~GlobalDummyWindow() {
		isDummy = 0;
		onDestroyWindow();
	}
};

GlobalDummyWindow gameWindow;

void onCreateWindow(HWND hWnd) {


	// ウィンドウハンドルを保存
	hNB10Wnd = hWnd;

	// 呼び出し元(WinMM.dll)に、「今回の起動ではちゃんとウィンドウが生成されたよ」と伝える。よってゲーム本体が起動された。
	*referenceOfNB10Wnd = hWnd;

	OutputDebugStream("ウィンドウ生成\n");

	OutputDebugStream("蒼天録の開始\n");
}


void onDestroyWindow() {
	OutputDebugStream("ウィンドウ破棄\r\n");
}
