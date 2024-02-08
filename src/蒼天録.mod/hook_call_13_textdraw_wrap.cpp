#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged

/*
007EC140   6A FF            PUSH -1
007EC142   68 FB698300      PUSH Nobunaga.008369FB                   ; ASCII "�T��E; �\������O����ׂ̲ݽİ�
007EC147   64:A1 00000000   MOV EAX,DWORD PTR FS:[0]
007EC14D   50               PUSH EAX
007EC14E   64:8925 00000000 MOV DWORD PTR FS:[0],ESP
007EC155   83EC 68          SUB ESP,68


�Ăяo����
007EC5AE   53               PUSH EBX
007EC5AF   8D4C24 14        LEA ECX,DWORD PTR SS:[ESP+14]
007EC5B3   51               PUSH ECX
007EC5B4   8BCE             MOV ECX,ESI
007EC5B6   E8 85FBFFFF      CALL Nobunaga.007EC140
007EC5BB   6A 0C            PUSH 0C

�Ăяo����
007EC71E   6A 00            PUSH 0
007EC720   50               PUSH EAX
007EC721   C74424 1C 000000>MOV DWORD PTR SS:[ESP+1C],0
007EC729   E8 12FAFFFF      CALL Nobunaga.007EC140
007EC72E   8D4C24 04        LEA ECX,DWORD PTR SS:[ESP+4]


�ȏォ������͂Q�Ǝv����B

*/




int nTextDraw13WrapEaxBackup = 0;
int nTextDraw13WrapArg1 = 0;
int nTextDraw13WrapArg2 = 0;

void OnSSRExeTextDraw13WrapExecute() {
	OutputDebugStream("��������TextDraw13Wrap��������!!!��\n");
	/*
	OutputDebugStream("��������nTextDraw13WrapArg1 %x��\n", nTextDraw13WrapArg1);
	OutputDebugStream("��������nTextDraw13WrapArg2 %x��\n", nTextDraw13WrapArg2);
	*/
}


/*
007EC140   6A FF            PUSH -1
007EC142   68 FB698300      PUSH Nobunaga.008369FB                   ; ASCII "�T��E; �\������O����ׂ̲ݽİ�
007EC147   64:A1 00000000   MOV EAX,DWORD PTR FS:[0]

*/

int pSSRExeJumpFromToOnSSRExeTextDraw13Wrap = 0x7EC140; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw13Wrap�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeTextDraw13Wrap = 0x7EC147; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13Wrap() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		push - 1          // ���X�������L�q��������
		PUSH 0x8369FB    // ���X�������L�q��������

		mov nTextDraw13WrapEaxBackup, eax          // ��ŕ������邽�߁A�����EAX�͎���Ă���

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 5�Ԗڂ̈������擾
		mov nTextDraw13WrapArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 4�Ԗڂ̈������擾
		mov nTextDraw13WrapArg1, eax

		mov eax, nTextDraw13WrapEaxBackup		     // EAX�𕜌�����

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDraw13WrapExecute();

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

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13Wrap
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw13Wrap[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw13Wrap() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw13Wrap;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13Wrap + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw13Wrap + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13Wrap), cmdOnSSRExeJumpFromTextDraw13Wrap, 7, NULL);
	//7�o�C�g�̂ݏ�������
	// 007E2E60  /$ 6A FF          PUSH -1
	// 007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; �\������O����ׂ̲ݽİ�
	// �̂Q������ׂ��̂łV�o�C�g�����Ă���
}

#pragma managed
