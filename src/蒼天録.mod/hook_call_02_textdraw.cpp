/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A���̂Q

00552BA4   53               PUSH EBX
00552BA5   8D5424 5C        LEA EDX,DWORD PTR SS:[ESP+5C]
00552BA9   52               PUSH EDX
00552BAA   6A 14            PUSH 14
00552BAC   68 FFFFFF00      PUSH 0FFFFFF
00552BB1   C74424 68 160000>MOV DWORD PTR SS:[ESP+68],16
00552BB9   C74424 6C BD0000>MOV DWORD PTR SS:[ESP+6C],0BD
00552BC1   50               PUSH EAX
00552BC2   E8 99022900      CALL Nobunaga.007E2E60
00552BC7   8B5424 10        MOV EDX,DWORD PTR SS:[ESP+10]
00552BCB   53               PUSH EBX
00552BCC   8D4C24 5C        LEA ECX,DWORD PTR SS:[ESP+5C]
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw02Execute() {
	OutputDebugStream("��������TextDraw02��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw02 = 0x552BC2; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw02�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw02 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw02 = 0x552BC7; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw02() {
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

	OnSSRExeTextDraw02Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw02 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw02
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw02[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw02() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw02;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw02 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw02 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw02), cmdOnSSRExeJumpFromTextDraw02, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
