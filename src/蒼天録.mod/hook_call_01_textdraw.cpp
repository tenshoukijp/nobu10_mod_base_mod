/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂P

00552B73   8B4C24 10        MOV ECX,DWORD PTR SS:[ESP+10]
00552B77   53               PUSH EBX
00552B78   8D4424 5C        LEA EAX,DWORD PTR SS:[ESP+5C]
00552B7C   50               PUSH EAX
00552B7D   6A 14            PUSH 14
00552B7F   68 FFFFFF00      PUSH 0FFFFFF
00552B84   51               PUSH ECX
00552B85   8B4E 04          MOV ECX,DWORD PTR DS:[ESI+4]
00552B88   C74424 6C 140000>MOV DWORD PTR SS:[ESP+6C],14
00552B90   C74424 70 0B0000>MOV DWORD PTR SS:[ESP+70],0B
00552B98   E8 C3022900      CALL Nobunaga.007E2E60
00552B9D   8B4424 10        MOV EAX,DWORD PTR SS:[ESP+10]
00552BA1   8B4E 04          MOV ECX,DWORD PTR DS:[ESI+4]
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw01Execute() {
	OutputDebugStream("��������TextDraw01��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw01 = 0x552B98; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw01�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw01 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw01 = 0x552B9D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw01() {
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

	OnSSRExeTextDraw01Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw01 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw01
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw01[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw01() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw01;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw01 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw01 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw01), cmdOnSSRExeJumpFromTextDraw01, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
