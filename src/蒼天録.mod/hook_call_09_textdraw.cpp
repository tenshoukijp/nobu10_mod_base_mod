/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂X
(��for���[�v�̒��ɏ����������Ă���\��������߂č���)

007D511A  |. 8B45 04        ||MOV EAX,DWORD PTR SS:[EBP+4]
007D511D  |. 2B4424 28      ||SUB EAX,DWORD PTR SS:[ESP+28]
007D5121  |. 99             ||CDQ
007D5122  |. 2BC2           ||SUB EAX,EDX
007D5124  |. D1F8           ||SAR EAX,1
007D5126  |> 014424 20      ||ADD DWORD PTR SS:[ESP+20],EAX
007D512A  |> 8B46 10        ||MOV EAX,DWORD PTR DS:[ESI+10]
007D512D  |. 8B4E 0C        ||MOV ECX,DWORD PTR DS:[ESI+C]
007D5130  |. 8B56 08        ||MOV EDX,DWORD PTR DS:[ESI+8]
007D5133  |. 6A 00          ||PUSH 0
007D5135  |. 8D7C24 20      ||LEA EDI,DWORD PTR SS:[ESP+20]
007D5139  |. 57             ||PUSH EDI
007D513A  |. 50             ||PUSH EAX
007D513B  |. 51             ||PUSH ECX
007D513C  |. 8B4C24 68      ||MOV ECX,DWORD PTR SS:[ESP+68]
007D5140  |. 52             ||PUSH EDX
007D5141  |. E8 1ADD0000    ||CALL Nobunaga.007E2E60
007D5146  |. 8B5C24 14      ||MOV EBX,DWORD PTR SS:[ESP+14]
007D514A  |. 8B4424 60      ||MOV EAX,DWORD PTR SS:[ESP+60]
007D514E  |. 83C6 18        ||ADD ESI,18
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw09Execute() {
	OutputDebugStream("��������TextDraw09��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw09 = 0x7D5141; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw09�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw09 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw09 = 0x7D5146; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw09() {
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

	OnSSRExeTextDraw09Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw09 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw09
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw09[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw09() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw09;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw09 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw09 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw09), cmdOnSSRExeJumpFromTextDraw09, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
