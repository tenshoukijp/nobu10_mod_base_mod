/*
�T�˒ʏ�̕�����͂قƂ�ǁu����14�ԁv����\������Ă���B
�������A��`�������o�R�ŕ\������Ă���̂Ŗ��f���Ȃ�Ȃ��B
*/

/*
0x07E2E60 = textdraw �̃��b�v�֐����Ăяo���Ă���ꏊ�A����14

007F7049  |. 8970 20        MOV DWORD PTR DS:[EAX+20],ESI
007F704C  |> C746 20 000000>MOV DWORD PTR DS:[ESI+20],0
007F7053  |. 8975 48        MOV DWORD PTR SS:[EBP+48],ESI
007F7056  |> 8B46 1C        MOV EAX,DWORD PTR DS:[ESI+1C]
007F7059  |. 85C0           TEST EAX,EAX
007F705B  |. 75 03          JNZ SHORT Nobunaga.007F7060
007F705D  |. 8975 44        MOV DWORD PTR SS:[EBP+44],ESI
007F7060  |> 8B4C24 28      MOV ECX,DWORD PTR SS:[ESP+28]
007F7064  |. 8B5424 24      MOV EDX,DWORD PTR SS:[ESP+24]
007F7068  |. 6A 00          PUSH 0
007F706A  |. 8D4424 18      LEA EAX,DWORD PTR SS:[ESP+18]
007F706E  |. 50             PUSH EAX
007F706F  |. 8B4424 28      MOV EAX,DWORD PTR SS:[ESP+28]
007F7073  |. 51             PUSH ECX
007F7074  |. 52             PUSH EDX
007F7075  |. 50             PUSH EAX
007F7076  |. 8BCD           MOV ECX,EBP
007F7078  |. E8 E3BDFEFF    CALL Nobunaga.007E2E60
007F707D  |. 85C0           TEST EAX,EAX

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw14Execute() {
	// OutputDebugStream("��������TextDraw14��������!!!��\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw14 = 0x7F7078; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw14�ւƃW�����v���Ă���B
int pSSRExeJumpCallFromToOnSSRExeTextDraw14 = 0x7E2E60; // ���X������������Call��
int pSSRExeReturnLblFromOnSSRExeTextDraw14 = 0x7F707D; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw14() {
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

	OnSSRExeTextDraw14Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw14 // ���̏���

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw14
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw14[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw14() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw14;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw14 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw14 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw14), cmdOnSSRExeJumpFromTextDraw14, 5, NULL); //5�o�C�g�̂ݏ�������
}

#pragma managed
