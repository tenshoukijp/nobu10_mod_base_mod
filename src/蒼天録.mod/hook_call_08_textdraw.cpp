/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂W

00552CD2  |. 53             PUSH EBX
00552CD3  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552CD7  |. 51             PUSH ECX
00552CD8  |. 6A 14          PUSH 14
00552CDA  |. C74424 64 1700>MOV DWORD PTR SS:[ESP+64],17
00552CE2  |. C74424 68 6102>MOV DWORD PTR SS:[ESP+68],261
00552CEA  |. 68 FFFFFF00    PUSH 0FFFFFF
00552CEF  |. 8B5424 24      MOV EDX,DWORD PTR SS:[ESP+24]
00552CF3  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CF6  |. 52             PUSH EDX
00552CF7  |. E8 64012900    CALL Nobunaga.007E2E60
00552CFC  |. 8B45 0C        MOV EAX,DWORD PTR SS:[EBP+C]
00552CFF  |. 50             PUSH EAX
00552D00  |. 8D4C24 34      LEA ECX,DWORD PTR SS:[ESP+34]
00552D04  |. 51             PUSH ECX

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw08Execute() {
	OutputDebugStream("��������TextDraw08��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw08 = 0x552CF7; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw08�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw08 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw08 = 0x552CFC; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw08() {
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

	OnSSRExeTextDraw08Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw08 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw08
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw08[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw08() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw08;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw08 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw08 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw08), cmdOnSSRExeJumpFromTextDraw08, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
