/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����15

007F7328   . 8B77 44        MOV ESI,DWORD PTR DS:[EDI+44]
007F732B   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]
007F732E   . 74 22          JE SHORT Nobunaga.007F7352
007F7330   > 8B4E 18        MOV ECX,DWORD PTR DS:[ESI+18]
007F7333   . 8B56 14        MOV EDX,DWORD PTR DS:[ESI+14]
007F7336   . 8B46 10        MOV EAX,DWORD PTR DS:[ESI+10]
007F7339   . 6A 00          PUSH 0
007F733B   . 56             PUSH ESI
007F733C   . 51             PUSH ECX
007F733D   . 52             PUSH EDX
007F733E   . 50             PUSH EAX
007F733F   . 8BCF           MOV ECX,EDI
007F7341   . E8 1ABBFEFF    CALL Nobunaga.007E2E60
007F7346   . 85C0           TEST EAX,EAX
007F7348   . 7C 10          JL SHORT Nobunaga.007F735A
007F734A   . 8B76 20        MOV ESI,DWORD PTR DS:[ESI+20]
007F734D   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"


#pragma unmanaged


void OnSSRExeTextDraw15Execute() {
	OutputDebugStream("��������TextDraw15��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw15 = 0x7F7341; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw15�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw15 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw15 = 0x7F7346; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw15() {
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

	OnSSRExeTextDraw15Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw15 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw15
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw15[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw15() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw15;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw15 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw15 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw15), cmdOnSSRExeJumpFromTextDraw15, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
