/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����11

007E319F  |. 8B46 08        MOV EAX,DWORD PTR DS:[ESI+8]
007E31A2  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
007E31A5  |. 53             PUSH EBX
007E31A6  |. 8D5424 10      LEA EDX,DWORD PTR SS:[ESP+10]
007E31AA  |. 52             PUSH EDX
007E31AB  |. 8B16           MOV EDX,DWORD PTR DS:[ESI]
007E31AD  |. 50             PUSH EAX
007E31AE  |. 51             PUSH ECX
007E31AF  |. 52             PUSH EDX
007E31B0  |. 8BCF           MOV ECX,EDI
007E31B2  |. 895C24 20      MOV DWORD PTR SS:[ESP+20],EBX
007E31B6  |. 895C24 24      MOV DWORD PTR SS:[ESP+24],EBX
007E31BA  |. E8 A1FCFFFF    CALL Nobunaga.007E2E60

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw11Execute() {
	OutputDebugStream("��������TextDraw11��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw11 = 0x7F7078; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw11�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw11 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw11 = 0x7E31BF; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw11() {
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

	OnSSRExeTextDraw11Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw11 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw11
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw11[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw11() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw11;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw11 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw11 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw11), cmdOnSSRExeJumpFromTextDraw11, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
