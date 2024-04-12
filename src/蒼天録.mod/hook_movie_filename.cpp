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


MOVIEの文字列へのポインタ配列 [0x88125C] = "MOVIE\OP.MPG"のこと
00929B90  7C 12 88 00 6C 12 88 00 [5C 12 88 00] [4C 12 88 00]  |・l・\・L・ 最後の２つはMPGっぽい


/*
動画の再生ポインタ。0x929B98 = [5C 12 88 00] であり、0x88125C = MOVIE\OP.MPG の文字列へのポインタ
004B063A   83F8 02          CMP EAX,2
004B063D   0F8D 20010000    JGE Nobunaga.004B0763
004B0643   8B1485 989B9200  MOV EDX,DWORD PTR DS:[EAX*4+929B98]
004B064A   85D2             TEST EDX,EDX
*/


int iMovieFileNameCharPointer = NULL;

static char bufOverrideMovieName[512] = "";

void OnSSRExeMovieFileNameExecute() {
	OutputDebugStream("★★★Movieが通りますよ:");
	OutputDebugStream("値 %x\n", iMovieFileNameCharPointer);
	OutputDebugStream("値 %s\n", iMovieFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iMovieFileNameCharPointer != NULL) {
		// OVERRIDEフォルダに対応するファイルがあれば、それを採用する
		string overrideMovieName = string("OVERRIDE\\") + string((char*)iMovieFileNameCharPointer);
		if (isFileExists(overrideMovieName)) {
			OutputDebugStream("オーバーライドファイルが存在するので、それを使う\n");
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

int pSSRExeJumpFromToOnSSRExeMovieFileName = 0x4B0643; // 関数はこのアドレスから、OnSSRExeMovieFileNameへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeMovieFileName = 0x4B064A; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeMovieFileName() {
	// スタックにためておく
	__asm {
		MOV EDX, DWORD PTR DS : [EAX * 4 + 0x929B98] // 元々の処理をここに
		mov iMovieFileNameCharPointer, EDX // ファイル名へのポインタを保存しておく
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

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		MOV EDX, iMovieFileNameCharPointer; // ファイルポインタを上書きしているかもしれないのでESIに転写(上書きしてなければ元々EAXと同じ値が入っている)

		jmp pSSRExeReturnLblFromOnSSRExeMovieFileName
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromMovieFileName[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeMovieFileName() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeMovieFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeMovieFileName + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromMovieFileName + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeMovieFileName), cmdOnSSRExeJumpFromMovieFileName, 7, NULL);
	//7バイトのみ書き込む
	// 004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]　が 7バイトあるので綺麗に書き換えておく
}

#pragma managed
