/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����12

007EB92B  |. 8946 14        MOV DWORD PTR DS:[ESI+14],EAX
007EB92E  |> 8B4E 14        MOV ECX,DWORD PTR DS:[ESI+14]
007EB931  |. 8B56 10        MOV EDX,DWORD PTR DS:[ESI+10]
007EB934  |. 6A 01          PUSH 1
007EB936  |. 8D4424 08      LEA EAX,DWORD PTR SS:[ESP+8]
007EB93A  |. 50             PUSH EAX
007EB93B  |. 8B46 0C        MOV EAX,DWORD PTR DS:[ESI+C]
007EB93E  |. 51             PUSH ECX
007EB93F  |. 8B4E 08        MOV ECX,DWORD PTR DS:[ESI+8]
007EB942  |. 52             PUSH EDX
007EB943  |. 50             PUSH EAX
007EB944  |. C74424 18 0000>MOV DWORD PTR SS:[ESP+18],0
007EB94C  |. C74424 1C 0000>MOV DWORD PTR SS:[ESP+1C],0
007EB954  |. E8 0775FFFF    CALL Nobunaga.007E2E60
007EB959  |. 8B46 20        MOV EAX,DWORD PTR DS:[ESI+20]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw12Execute() {
	OutputDebugStream("��������TextDraw12��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw12 = 0x7EB954; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw12�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw12 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw12 = 0x7EB959; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw12() {
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

	OnSSRExeTextDraw12Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw12 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw12
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw12[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw12() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw12;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw12 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw12 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw12), cmdOnSSRExeJumpFromTextDraw12, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
