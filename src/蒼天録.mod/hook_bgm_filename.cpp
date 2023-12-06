#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged


/*
00881256  50 47 00 00 00 00 4D 4F 56 49 45 5C 4F 50 2E 4D  PG....MOVIE\OP.M
00881266  50 47 00 00 00 00 42 47 4D 5C 42 47 4D 32 39 2E  PG....BGM\BGM29.
00881276  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 38 2E  NBX...BGM\BGM28.
00881286  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 37 2E  NBX...BGM\BGM27.
00881296  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 36 2E  NBX...BGM\BGM26.
008812A6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 35 2E  NBX...BGM\BGM25.
008812B6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 34 2E  NBX...BGM\BGM24.
008812C6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 33 2E  NBX...BGM\BGM23.
008812D6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 32 2E  NBX...BGM\BGM22.
008812E6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 31 2E  NBX...BGM\BGM21.
008812F6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 32 30 2E  NBX...BGM\BGM20.
00881306  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 39 2E  NBX...BGM\BGM19.
00881316  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 38 2E  NBX...BGM\BGM18.
00881326  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 37 2E  NBX...BGM\BGM17.
00881336  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 36 2E  NBX...BGM\BGM16.
00881346  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 35 2E  NBX...BGM\BGM15.
00881356  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 34 2E  NBX...BGM\BGM14.
00881366  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 33 2E  NBX...BGM\BGM13.
00881376  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 32 2E  NBX...BGM\BGM12.
00881386  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 31 2E  NBX...BGM\BGM11.
00881396  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 31 30 2E  NBX...BGM\BGM10.
008813A6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 39 2E  NBX...BGM\BGM09.
008813B6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 38 2E  NBX...BGM\BGM08.
008813C6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 37 2E  NBX...BGM\BGM07.
008813D6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 36 2E  NBX...BGM\BGM06.
008813E6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 35 2E  NBX...BGM\BGM05.
008813F6  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 34 2E  NBX...BGM\BGM04.
00881406  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 33 2E  NBX...BGM\BGM03.
00881416  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 32 2E  NBX...BGM\BGM02.
00881426  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 31 2E  NBX...BGM\BGM01.
00881436  4E 42 58 00 00 00 42 47 4D 5C 42 47 4D 30 30 2E  NBX...BGM\BGM00.
00881446  4E 42 58 00 00 00 53 45 5C 53 45 33 30 38 2E 57  NBX...SE\SE308.W


BGM�̕�����ւ̃|�C���^�z�� [0x88143C] = BGM\BGM00.NBX�̂���
00929B20  3C 14 88 00 2C 14 88 00 1C 14 88 00 0C 14 88 00  <�E,�E�E.�E
00929B30  FC 13 88 00 EC 13 88 00 DC 13 88 00 CC 13 88 00  �E�E�E�E��E��E
00929B40  BC 13 88 00 AC 13 88 00 9C 13 88 00 8C 13 88 00  ��E��E�E�E�E�E
00929B50  7C 13 88 00 6C 13 88 00 5C 13 88 00 4C 13 88 00  |�El�E\�EL�E
00929B60  3C 13 88 00 2C 13 88 00 1C 13 88 00 0C 13 88 00  <�E,�E�E.�E
00929B70  FC 12 88 00 EC 12 88 00 DC 12 88 00 CC 12 88 00  �E�E�E�E��E��E
00929B80  BC 12 88 00 AC 12 88 00 9C 12 88 00 8C 12 88 00  ��E��E�E�E�E�E
00929B90  7C 12 88 00 6C 12 88 00 5C 12 88 00 4C 12 88 00  |�El�E\�EL�E �Ō�̂Q��MPG���ۂ�


BGM�̍Đ��|�C���^
004B028E   7D 4C            JGE SHORT Nobunaga.004B02DC
004B0290   56               PUSH ESI
004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]
004B0298   85F6             TEST ESI,ESI

*/


int iBGMFileNameCharPointer = NULL;

char bufOverrideMovieName[512] = "";

void OnSSRExeBGMFileNameExecute() {
	OutputDebugStream("������BGM���ʂ�܂���:");
	OutputDebugStream("�l %x\n", iBGMFileNameCharPointer);
	OutputDebugStream("�l %s\n", iBGMFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iBGMFileNameCharPointer != NULL) {
		// JS����̏㏑���t�@�C���p�X�̎w������������A������̗p����
		callJSModRequestBGM((char*)iBGMFileNameCharPointer, bufOverrideMovieName);
		if (strlen(bufOverrideMovieName) > 0) {
			OutputDebugStream("JS����̎w���t�@�C�����������̂ŁA������g��\n");
			iBGMFileNameCharPointer = (int)bufOverrideMovieName;
		}
		else {
			OutputDebugStream("JS����̎w���t�@�C���͂Ȃ������̂ŁA�I�[�o�[���C�h���`�F�b�N\n");
			// OVERRIDE�t�H���_�ɑΉ�����t�@�C��������΁A������̗p����
			string overrideMovieName = string("OVERRIDE\\") + string((char*)iBGMFileNameCharPointer);
			if (isFileExists(overrideMovieName)) {
				OutputDebugStream("�I�[�o�[���C�h�t�@�C�������݂���̂ŁA������g��\n");
				strcpy_s(bufOverrideMovieName, overrideMovieName.c_str());
				iBGMFileNameCharPointer = (int)bufOverrideMovieName;
			}
		}
	}
}


/*
004B0290   56               PUSH ESI
004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]
004B0298   85F6             TEST ESI,ESI
*/

int pSSRExeJumpFromToOnSSRExeBGMFileName = 0x4B0291; // �֐��͂��̃A�h���X����AOnSSRExeBGMFileName�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeBGMFileName = 0x4B0298; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeBGMFileName() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		MOV ESI, DWORD PTR DS : [EAX * 4 + 0x929B20] // ���X�̏�����������
		mov iBGMFileNameCharPointer, ESI // �t�@�C�����ւ̃|�C���^��ۑ����Ă���
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeBGMFileNameExecute();

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

		MOV ESI, iBGMFileNameCharPointer; // �t�@�C���|�C���^���㏑�����Ă��邩������Ȃ��̂�ESI�ɓ]��(�㏑�����ĂȂ���Ό��XEAX�Ɠ����l�������Ă���)

		jmp pSSRExeReturnLblFromOnSSRExeBGMFileName
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromBGMFileName[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeBGMFileName() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeBGMFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeBGMFileName + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromBGMFileName + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeBGMFileName), cmdOnSSRExeJumpFromBGMFileName, 7, NULL);
	//7�o�C�g�̂ݏ�������
	// 004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]�@�� 7�o�C�g����̂��Y��ɏ��������Ă���
}

#pragma managed
