/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����16

007F73C9   . 6A 00          PUSH 0
007F73CB   . 56             PUSH ESI
007F73CC   . 51             PUSH ECX
007F73CD   . 52             PUSH EDX
007F73CE   . 50             PUSH EAX
007F73CF   . 8BCF           MOV ECX,EDI
007F73D1   . E8 8ABAFEFF    CALL Nobunaga.007E2E60
007F73D6   . 85C0           TEST EAX,EAX
007F73D8   . 7C 10          JL SHORT Nobunaga.007F73EA
007F73DA   . 8B76 20        MOV ESI,DWORD PTR DS:[ESI+20]
007F73DD   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw16Execute() {
	OutputDebugStream("��������TextDraw16��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw16 = 0x7F73D1; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw16�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw16 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw16 = 0x7F73D6; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw16() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDraw16Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw16 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw16
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw16[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw16() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw16;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw16 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw16 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw16), cmdOnSSRExeJumpFromTextDraw16, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
