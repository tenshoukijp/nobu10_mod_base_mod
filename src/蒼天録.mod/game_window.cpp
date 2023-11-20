#include <windows.h>
#include "game_font.h"
#include "output_debug_stream.h"

HWND hNB10Wnd = NULL;

HWND* referenceOfNB10Wnd = NULL;

// dll���j�����ꂽ���ɁAonDestroyWindow�����s���邽�߁B
extern void onDestroyWindow();
class GlobalDummyWindow {
	int isDummy;
public:
	GlobalDummyWindow() {
		isDummy = 0;
		OutputDebugStream("�_�~�[�C���X�^���X����\n");
	}
	~GlobalDummyWindow() {
		isDummy = 0;
		onDestroyWindow();
	}
};

GlobalDummyWindow gameWindow;

void onCreateWindow(HWND hWnd) {


	// �E�B���h�E�n���h����ۑ�
	hNB10Wnd = hWnd;

	// �Ăяo����(WinMM.dll)�ɁA�u����̋N���ł͂����ƃE�B���h�E���������ꂽ��v�Ɠ`����B����ăQ�[���{�̂��N�����ꂽ�B
	*referenceOfNB10Wnd = hWnd;

	OutputDebugStream("�E�B���h�E����\n");

	OutputDebugStream("���V�^�̊J�n\n");
}


void onDestroyWindow() {
	OutputDebugStream("�E�B���h�E�j��\r\n");
}
