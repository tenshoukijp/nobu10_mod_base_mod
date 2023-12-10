#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged

/*
������\���̂��߂̊�{�֐����Ƃ�������
007E2E60  /$ 6A FF          PUSH -1
007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; �\������O����ׂ̲ݽİ�
007E2E67  |. 64:A1 00000000 MOV EAX,DWORD PTR FS:[0]
007E2E6D  |. 50             PUSH EAX
007E2E6E  |. 64:8925 000000>MOV DWORD PTR FS:[0],ESP
007E2E75  |. 83EC 10        SUB ESP,10
007E2E78  |. 8B4424 28      MOV EAX,DWORD PTR SS:[ESP+28]
007E2E7C  |. 55             PUSH EBP
007E2E7D  |. 57             PUSH EDI
007E2E7E  |. 8BF9           MOV EDI,ECX
007E2E80  |. 8B0D 60D8AC00  MOV ECX,DWORD PTR DS:[ACD860]
007E2E86  |. 50             PUSH EAX
007E2E87  |. 897C24 14      MOV DWORD PTR SS:[ESP+14],EDI
007E2E8B  |. E8 40CDFEFF    CALL Nobunaga.007CFBD0
007E2E90  |. 33ED           XOR EBP,EBP
007E2E92  |. 3BC5           CMP EAX,EBP


��̊֐����g���Ă���Ƃ���͉��L
�Q�ƃf�[�^ �F Nobunaga:.text -> 007E2E60
���ڽ      �ި�������                                ����
00552B98   CALL Nobunaga.007E2E60
00552BC2   CALL Nobunaga.007E2E60
00552BEC   CALL Nobunaga.007E2E60
00552C16   CALL Nobunaga.007E2E60
00552C79   CALL Nobunaga.007E2E60
00552CA3   CALL Nobunaga.007E2E60
00552CCD   CALL Nobunaga.007E2E60
00552CF7   CALL Nobunaga.007E2E60
007D5141   CALL Nobunaga.007E2E60
007E312E   CALL Nobunaga.007E2E60
007E31BA   CALL Nobunaga.007E2E60
007EB954   CALL Nobunaga.007E2E60
007EC474   CALL Nobunaga.007E2E60
007F7078   CALL Nobunaga.007E2E60
007F7341   CALL Nobunaga.007E2E60
007F73D1   CALL Nobunaga.007E2E60


// ���̂悤�ȂT�̈����̎g���������Ă���B
007F73C9   . 6A 00          PUSH 0                                                    ; /Arg5 = 00000000
007F73CB   . 56             PUSH ESI                                                  ; |Arg4
007F73CC   . 51             PUSH ECX                                                  ; |Arg3
007F73CD   . 52             PUSH EDX                                                  ; |Arg2
007F73CE   . 50             PUSH EAX                                                  ; |Arg1
007F73D1   . E8 8ABAFEFF    CALL Nobunaga.007E2E60                                    ; \Nobunaga.007E2E60



*/




int nTextDrawEaxBackup = 0;
int nTextDrawArg1 = 0;
int nTextDrawArg2 = 0;
int nTextDrawArgTargetString = 0; // ���ꂪ������̃A�h���X
int nTextDrawArg4 = 0;
int nTextDrawArg5 = 0;

void OnSSRExeTextDrawExecute() {
	if (nTextDrawArgTargetString != NULL && strlen((char *)nTextDrawArgTargetString) > 0) {
		OutputDebugStream("TextDraw���ʂ�܂���");
		OutputDebugStream("nTextDrawArg1%x\n", nTextDrawArg1);
		OutputDebugStream("nTextDrawArg2%x\n", nTextDrawArg2);
		OutputDebugStream("nTextDrawArg3%x\n", nTextDrawArgTargetString);
		OutputDebugStream("��nTextDrawArgSTR%s\n", nTextDrawArgTargetString);
		OutputDebugStream("nTextDrawArg4%x\n", nTextDrawArg4);
		OutputDebugStream("nTextDrawArg5%x\n", nTextDrawArg5);
	}
}


/*
007E2E60  /$ 6A FF          PUSH -1
007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; �\������O����ׂ̲ݽİ�
007E2E67  |. 64:A1 00000000 MOV EAX,DWORD PTR FS:[0]

*/

int pSSRExeJumpFromToOnSSRExeTextDraw = 0x7E2E60; // �֐��͂��̃A�h���X����AOnSSRExeTextDraw�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeTextDraw = 0x7E2E67; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		push -1          // ���X�������L�q��������
		PUSH 0x8364F8    // ���X�������L�q��������

		mov nTextDrawEaxBackup, eax          // ��ŕ������邽�߁A�����EAX�͎���Ă���

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 5�Ԗڂ̈������擾
		mov nTextDrawArg5, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 4�Ԗڂ̈������擾
		mov nTextDrawArg4, eax
		MOV eax, DWORD PTR SS : [ESP + 0xC] // 3�Ԗڂ̈������擾
		mov nTextDrawArgTargetString, eax
		MOV eax, DWORD PTR SS : [ESP + 0x10] // 2�Ԗڂ̈������擾
		mov nTextDrawArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0x14] // 1�Ԗڂ̈������擾
		mov nTextDrawArg1, eax

		mov eax, nTextDrawEaxBackup		     // EAX�𕜌�����



		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDrawExecute();

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

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromTextDraw[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeTextDraw() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeTextDraw;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromTextDraw + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw), cmdOnSSRExeJumpFromTextDraw, 7, NULL);
	//7�o�C�g�̂ݏ�������
	// 007E2E60  /$ 6A FF          PUSH -1
	// 007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; �\������O����ׂ̲ݽİ�
	// �̂Q������ׂ��̂łV�o�C�g�����Ă���
}

#pragma managed
