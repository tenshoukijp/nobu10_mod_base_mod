#include <windows.h>
#include "game_font.h"
#include "javascript_mod.h"
#include "output_debug_stream.h"

HWND hNB10Wnd = NULL;

HWND* referenceOfNB10Wnd = NULL;


void onCreateWindow(HWND hWnd) {


	// �E�B���h�E�n���h����ۑ�
	hNB10Wnd = hWnd;

	// �Ăяo����(WinMM.dll)�ɁA�u����̋N���ł͂����ƃE�B���h�E���������ꂽ��v�Ɠ`����B����ăQ�[���{�̂��N�����ꂽ�B
	*referenceOfNB10Wnd = hWnd;

	// JavaScriptMod
	callJSModCreateWindow(hWnd);

	OutputDebugStream("�E�B���h�E����\n");

	OutputDebugStream("���V�^�̊J�n\n");
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		// JavaScriptMod
		callJSModDestroyWindow();

		OutputDebugStream("�E�B���h�E�j��\r\n");
	}

	doneDestroyWindow = TRUE;
}
