/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂R

00552BC7  |. 8B5424 10      MOV EDX,DWORD PTR SS:[ESP+10]
00552BCB  |. 53             PUSH EBX
00552BCC  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552BD0  |. 51             PUSH ECX
00552BD1  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552BD4  |. 6A 14          PUSH 14
00552BD6  |. 68 FFFFFF00    PUSH 0FFFFFF
00552BDB  |. 52             PUSH EDX
00552BDC  |. C74424 6C 1400>MOV DWORD PTR SS:[ESP+6C],14
00552BE4  |. C74424 70 6B01>MOV DWORD PTR SS:[ESP+70],16B
00552BEC  |. E8 6F022900    CALL Nobunaga.007E2E60
00552BF1  |. 8B4C24 10      MOV ECX,DWORD PTR SS:[ESP+10]
00552BF5  |. 53             PUSH EBX
00552BF6  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552BFA  |. 50             PUSH EAX
00552BFB  |. 6A 14          PUSH 14
*/

#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw03Execute() {
	OutputDebugStream("��������TextDraw03��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw03 = 0x552BEC; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw03�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw03 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw03 = 0x552BF1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw03() {
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

	OnSSRExeTextDraw03Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw03 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw03
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw03[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw03() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw03;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw03 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw03 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw03), cmdOnSSRExeJumpFromTextDraw03, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
