#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged


/*
00881246  74 72 6C 00 00 00 4D 4F 56 49 45 5C 45 44 2E 4D  trl...MOVIE\ED.M
00881256  50 47 00 00 00 00 4D 4F 56 49 45 5C 4F 50 2E 4D  PG....MOVIE\OP.M
00881266  50 47 00 00 00 00 42 47 4D 5C 42 47 4D 32 39 2E  PG....BGM\BGM29.


MOVIE�̕�����ւ̃|�C���^�z�� [0x88125C] = "MOVIE\OP.MPG"�̂���
00929B90  7C 12 88 00 6C 12 88 00 [5C 12 88 00] [4C 12 88 00]  |�El�E\�EL�E �Ō�̂Q��MPG���ۂ�


/*
����̍Đ��|�C���^�B0x929B98 = [5C 12 88 00] �ł���A0x88125C = MOVIE\OP.MPG �̕�����ւ̃|�C���^
004B063A   83F8 02          CMP EAX,2
004B063D   0F8D 20010000    JGE Nobunaga.004B0763
004B0643   8B1485 989B9200  MOV EDX,DWORD PTR DS:[EAX*4+929B98]
004B064A   85D2             TEST EDX,EDX
*/


int iMovieFileNameCharPointer = NULL;

static char bufOverrideMovieName[512] = "";

void OnSSRExeMovieFileNameExecute() {
	OutputDebugStream("������Movie���ʂ�܂���:");
	OutputDebugStream("�l %x\n", iMovieFileNameCharPointer);
	OutputDebugStream("�l %s\n", iMovieFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iMovieFileNameCharPointer != NULL) {
		// OVERRIDE�t�H���_�ɑΉ�����t�@�C��������΁A������̗p����
		string overrideMovieName = string("OVERRIDE\\") + string((char*)iMovieFileNameCharPointer);
		if (isFileExists(overrideMovieName)) {
			OutputDebugStream("�I�[�o�[���C�h�t�@�C�������݂���̂ŁA������g��\n");
			strcpy_s(bufOverrideMovieName, overrideMovieName.c_str());
			iMovieFileNameCharPointer = (int)bufOverrideMovieName;
		}
	}
}


/*
004B063A   83F8 02          CMP EAX,2
004B063D   0F8D 20010000    JGE Nobunaga.004B0763
004B0643   8B1485 989B9200  MOV EDX,DWORD PTR DS:[EAX*4+929B98]
004B064A   85D2             TEST EDX,EDX
*/

int pSSRExeJumpFromToOnSSRExeMovieFileName = 0x4B0643; // �֐��͂��̃A�h���X����AOnSSRExeMovieFileName�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeMovieFileName = 0x4B064A; // �֐����Ō�܂ł����ƁA���̎��sexe���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeMovieFileName() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		MOV EDX, DWORD PTR DS : [EAX * 4 + 0x929B98] // ���X�̏�����������
		mov iMovieFileNameCharPointer, EDX // �t�@�C�����ւ̃|�C���^��ۑ����Ă���
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeMovieFileNameExecute();

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

		MOV EDX, iMovieFileNameCharPointer; // �t�@�C���|�C���^���㏑�����Ă��邩������Ȃ��̂�ESI�ɓ]��(�㏑�����ĂȂ���Ό��XEAX�Ɠ����l�������Ă���)

		jmp pSSRExeReturnLblFromOnSSRExeMovieFileName
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromMovieFileName[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeMovieFileName() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeMovieFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeMovieFileName + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromMovieFileName + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂����sexe�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeMovieFileName), cmdOnSSRExeJumpFromMovieFileName, 7, NULL);
	//7�o�C�g�̂ݏ�������
	// 004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]�@�� 7�o�C�g����̂��Y��ɏ��������Ă���
}

#pragma managed
