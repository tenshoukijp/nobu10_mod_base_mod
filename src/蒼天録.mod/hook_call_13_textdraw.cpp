/*
�T�ˉ�ʉ��������ɂł�u��b�v�́A����13�Ԃ��S�����Ă���͗l�B
(��for���[�v�̒��ɏ����������Ă���\��������߂č���)
*/


/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����13

007EC44A  |> 897C24 20      |MOV DWORD PTR SS:[ESP+20],EDI
007EC44E  |. 897C24 18      |MOV DWORD PTR SS:[ESP+18],EDI
007EC452  |. EB 25          |JMP SHORT Nobunaga.007EC479
007EC454  |> 57             |PUSH EDI
007EC455  |. 8D4424 44      |LEA EAX,DWORD PTR SS:[ESP+44]
007EC459  |. 50             |PUSH EAX
007EC45A  |. 8B4424 24      |MOV EAX,DWORD PTR SS:[ESP+24]
007EC45E  |. 8B88 80000000  |MOV ECX,DWORD PTR DS:[EAX+80]
007EC464  |. 8B50 70        |MOV EDX,DWORD PTR DS:[EAX+70]
007EC467  |. 8B8424 9000000>|MOV EAX,DWORD PTR SS:[ESP+90]
007EC46E  |. 51             |PUSH ECX
007EC46F  |. 8B48 04        |MOV ECX,DWORD PTR DS:[EAX+4]
007EC472  |. 52             |PUSH EDX
007EC473  |. 53             |PUSH EBX
007EC474  |. E8 E769FFFF    |CALL Nobunaga.007E2E60
007EC479  |> 53             |PUSH EBX

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw13Execute() {
	// OutputDebugStream("��������TextDraw13��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw13 = 0x7EC474; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw13�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw13 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw13 = 0x7EC479; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13() {
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

	OnSSRExeTextDraw13Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw13 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw13[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw13() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw13;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw13 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13), cmdOnSSRExeJumpFromTextDraw13, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
