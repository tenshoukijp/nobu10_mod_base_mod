/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂U

00552C7E  |. 8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+14]
00552C82  |. 53             PUSH EBX
00552C83  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552C87  |. 50             PUSH EAX
00552C88  |. 6A 14          PUSH 14
00552C8A  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C8F  |. 51             PUSH ECX
00552C90  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C93  |. C74424 6C 1900>MOV DWORD PTR SS:[ESP+6C],19
00552C9B  |. C74424 70 0301>MOV DWORD PTR SS:[ESP+70],103
00552CA3  |. E8 B8012900    CALL Nobunaga.007E2E60
00552CA8  |. 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
00552CAC  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CAF  |. 53             PUSH EBX
00552CB0  |. 8D5424 5C      LEA EDX,DWORD PTR SS:[ESP+5C]

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw06Execute() {
	OutputDebugStream("��������TextDraw06��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw06 = 0x552CA3; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw06�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw06 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw06 = 0x552CA8; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw06() {
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

	OnSSRExeTextDraw06Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw06 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw06
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw06[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw06() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw06;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw06 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw06 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw06), cmdOnSSRExeJumpFromTextDraw06, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
