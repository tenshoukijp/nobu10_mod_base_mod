#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
#include "onigwrap.h"
using namespace std;

#pragma unmanaged

/*
���̏����́Ahook_call_13_textdraw �̏�����B���[�v���n�܂钼�O
007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
007EC1B7   895C24 3C        MOV DWORD PTR SS:[ESP+3C],EBX
007EC1BB   897424 14        MOV DWORD PTR SS:[ESP+14],ESI
007EC1BF   897424 20        MOV DWORD PTR SS:[ESP+20],ESI
007EC1C3   897424 28        MOV DWORD PTR SS:[ESP+28],ESI
007EC1C7   897424 18        MOV DWORD PTR SS:[ESP+18],ESI

*/

int TextDraw13Prev_ECX = 0;  // ���ꂪ���b�Z�[�W�ł���
int TextDraw13Prev_ESI = 0;
int TextDraw13Prev_EBX = 0;

char bufTextDraw13Prev[512] = "���ꂠ��Ȃ񂩁H\xA���������Ēu�������Ȃ񂩁H\xA�ق�܂Ȃ񂩁H �����Ȃ񂩁H";

void OnSSRExeTextDraw13PrevExecute() {
	if (TextDraw13Prev_ECX != NULL && strlen((char*)TextDraw13Prev_ECX) > 0) {
		OutputDebugStream("��TextDraw13Prev��������!!!��\n");
		OutputDebugStream("��TextDraw13Prev_ECX %x��\n", TextDraw13Prev_ECX);
		OutputDebugStream("��TextDraw13Prev_ECX STR %s��\n", TextDraw13Prev_ECX);
		OutputDebugStream("��TextDraw13Prev_ESI %x��\n", TextDraw13Prev_ESI);
		OutputDebugStream("��TextDraw13Prev_EBX %x��\n", TextDraw13Prev_EBX);
		/*
		if (OnigMatch((char*)TextDraw13Prev_ECX, "�ɂ����߂�")) {
			strcpy((char*)TextDraw13Prev_ECX, "���ꂠ��Ȃ񂩁H\xA�u����������\xA�ق�܂Ȃ񂩁H");
		}
		*/
	}
}


/*
007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
007EC1B7   895C24 3C        MOV DWORD PTR SS:[ESP+3C],EBX
007EC1BB   897424 14        MOV DWORD PTR SS:[ESP+14],ESI
007EC1BF   897424 20        MOV DWORD PTR SS:[ESP+20],ESI
007EC1C3   897424 28        MOV DWORD PTR SS:[ESP+28],ESI
007EC1C7   897424 18        MOV DWORD PTR SS:[ESP+18],ESI

*/

int pSSRExeJumpFromToOnSSRExeTextDraw13Prev = 0x7EC1AF; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw13Prev�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeTextDraw13Prev = 0x7EC1B7; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13Prev() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov TextDraw13Prev_ECX, ecx
		mov TextDraw13Prev_ESI, esi
		mov TextDraw13Prev_EBX, ebx

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDraw13PrevExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov ecx, TextDraw13Prev_ECX

		MOV DWORD PTR SS : [ESP + 0x10] , ECX // ���̋L�q��������
		MOV DWORD PTR SS : [ESP + 0x38] , ESI // ���̋L�q��������

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13Prev
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw13Prev[9] = "\xE9\x90\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw13Prev() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw13Prev;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13Prev + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw13Prev + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13Prev), cmdOnSSRExeJumpFromTextDraw13Prev, 8, NULL);
	/*
	8�o�C�g�̂ݏ�������
	007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
	007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
	�̂Q������ׂ��̂�8�o�C�g�����Ă���
	*/
}

#pragma managed
