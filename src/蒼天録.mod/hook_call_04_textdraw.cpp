/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂S

00552BF1  |. 8B4C24 10      MOV ECX,DWORD PTR SS:[ESP+10]
00552BF5  |. 53             PUSH EBX
00552BF6  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552BFA  |. 50             PUSH EAX
00552BFB  |. 6A 14          PUSH 14
00552BFD  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C02  |. 51             PUSH ECX
00552C03  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C06  |. C74424 6C 1400>MOV DWORD PTR SS:[ESP+6C],14
00552C0E  |. C74424 70 1B02>MOV DWORD PTR SS:[ESP+70],21B
00552C16  |. E8 45022900    CALL Nobunaga.007E2E60
00552C1B  |. 8B17           MOV EDX,DWORD PTR DS:[EDI]
00552C1D  |. 6A FF          PUSH -1
00552C1F  |. 8D4424 14      LEA EAX,DWORD PTR SS:[ESP+14]
00552C23  |. 50             PUSH EAX
00552C24  |. 6A 2F          PUSH 2F

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw04Execute() {
	OutputDebugStream("��������TextDraw04��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw04 = 0x552C16; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw04�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw04 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw04 = 0x552C1B; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw04() {
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

	OnSSRExeTextDraw04Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw04 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw04
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw04[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw04() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw04;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw04 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw04 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw04), cmdOnSSRExeJumpFromTextDraw04, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
