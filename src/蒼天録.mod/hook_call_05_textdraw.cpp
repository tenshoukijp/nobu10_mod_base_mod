/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂T

00552C51  |. 8B5424 20      MOV EDX,DWORD PTR SS:[ESP+20]
00552C55  |. 83C4 0C        ADD ESP,0C
00552C58  |. 53             PUSH EBX
00552C59  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552C5D  |. 51             PUSH ECX
00552C5E  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C61  |. 6A 14          PUSH 14
00552C63  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C68  |. 52             PUSH EDX
00552C69  |. C74424 6C 1700>MOV DWORD PTR SS:[ESP+6C],17
00552C71  |. C74424 70 5100>MOV DWORD PTR SS:[ESP+70],51
00552C79  |. E8 E2012900    CALL Nobunaga.007E2E60
00552C7E  |. 8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+14]
00552C82  |. 53             PUSH EBX
00552C83  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552C87  |. 50             PUSH EAX
00552C88  |. 6A 14          PUSH 14

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw05Execute() {
	OutputDebugStream("��������TextDraw05��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw05 = 0x552C79; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw05�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw05 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw05 = 0x552C7E; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw05() {
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

	OnSSRExeTextDraw05Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw05 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw05
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw05[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw05() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw05;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw05 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw05 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw05), cmdOnSSRExeJumpFromTextDraw05, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
