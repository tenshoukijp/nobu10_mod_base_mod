/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����10

007E3119   > 8B4F 10        MOV ECX,DWORD PTR DS:[EDI+10]
007E311C   . 8B57 0C        MOV EDX,DWORD PTR DS:[EDI+C]
007E311F   . 6A 00          PUSH 0
007E3121   . 8D47 14        LEA EAX,DWORD PTR DS:[EDI+14]
007E3124   . 50             PUSH EAX
007E3125   . 8B47 08        MOV EAX,DWORD PTR DS:[EDI+8]
007E3128   . 51             PUSH ECX
007E3129   . 8B4F 04        MOV ECX,DWORD PTR DS:[EDI+4]
007E312C   . 52             PUSH EDX
007E312D   . 50             PUSH EAX
007E312E   . E8 2DFDFFFF    CALL Nobunaga.007E2E60
007E3133   . 5F             POP EDI
007E3134   . 5E             POP ESI
007E3135   . 83C4 10        ADD ESP,10

*/

#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw10Execute() {
	OutputDebugStream("��������TextDraw10��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw10 = 0x7E312E; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw10�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw10 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw10 = 0x7E3133; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw10() {
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

	OnSSRExeTextDraw10Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw10 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw10
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw10[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw10() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw10;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw10 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw10 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw10), cmdOnSSRExeJumpFromTextDraw10, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
