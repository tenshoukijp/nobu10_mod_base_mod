
/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂V

00552CA8  |. 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
00552CAC  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CAF  |. 53             PUSH EBX
00552CB0  |. 8D5424 5C      LEA EDX,DWORD PTR SS:[ESP+5C]
00552CB4  |. 52             PUSH EDX
00552CB5  |. 6A 14          PUSH 14
00552CB7  |. 68 FFFFFF00    PUSH 0FFFFFF
00552CBC  |. 50             PUSH EAX
00552CBD  |. C74424 6C 1700>MOV DWORD PTR SS:[ESP+6C],17
00552CC5  |. C74424 70 B101>MOV DWORD PTR SS:[ESP+70],1B1
00552CCD  |. E8 8E012900    CALL Nobunaga.007E2E60
00552CD2  |. 53             PUSH EBX
00552CD3  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552CD7  |. 51             PUSH ECX

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw07Execute() {
	OutputDebugStream("��������TextDraw07��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw07 = 0x552CCD; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw07�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw07 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw07 = 0x552CD2; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw07() {
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

	OnSSRExeTextDraw07Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw07 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw07
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw07[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw07() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw07;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw07 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw07 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw07), cmdOnSSRExeJumpFromTextDraw07, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
