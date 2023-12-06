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


BGMの文字列へのポインタ配列 [0x88143C] = BGM\BGM00.NBXのこと
00929B20  3C 14 88 00 2C 14 88 00 1C 14 88 00 0C 14 88 00  <・,・・.・
00929B30  FC 13 88 00 EC 13 88 00 DC 13 88 00 CC 13 88 00  ・・・・ﾜ・ﾌ・
00929B40  BC 13 88 00 AC 13 88 00 9C 13 88 00 8C 13 88 00  ｼ・ｬ・・・・・
00929B50  7C 13 88 00 6C 13 88 00 5C 13 88 00 4C 13 88 00  |・l・\・L・
00929B60  3C 13 88 00 2C 13 88 00 1C 13 88 00 0C 13 88 00  <・,・・.・
00929B70  FC 12 88 00 EC 12 88 00 DC 12 88 00 CC 12 88 00  ・・・・ﾜ・ﾌ・
00929B80  BC 12 88 00 AC 12 88 00 9C 12 88 00 8C 12 88 00  ｼ・ｬ・・・・・
00929B90  7C 12 88 00 6C 12 88 00 5C 12 88 00 4C 12 88 00  |・l・\・L・ 最後の２つはMPGっぽい


BGMの再生ポインタ
004B028E   7D 4C            JGE SHORT Nobunaga.004B02DC
004B0290   56               PUSH ESI
004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]
004B0298   85F6             TEST ESI,ESI

*/


int iBGMFileNameCharPointer = NULL;

char bufOverrideMovieName[512] = "";

void OnSSRExeBGMFileNameExecute() {
	OutputDebugStream("★★★BGMが通りますよ:");
	OutputDebugStream("値 %x\n", iBGMFileNameCharPointer);
	OutputDebugStream("値 %s\n", iBGMFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iBGMFileNameCharPointer != NULL) {
		// JSからの上書きファイルパスの指示があったら、それを採用する
		callJSModRequestBGM((char*)iBGMFileNameCharPointer, bufOverrideMovieName);
		if (strlen(bufOverrideMovieName) > 0) {
			OutputDebugStream("JSからの指示ファイルがあったので、それを使う\n");
			iBGMFileNameCharPointer = (int)bufOverrideMovieName;
		}
		else {
			OutputDebugStream("JSからの指示ファイルはなかったので、オーバーライドをチェック\n");
			// OVERRIDEフォルダに対応するファイルがあれば、それを採用する
			string overrideMovieName = string("OVERRIDE\\") + string((char*)iBGMFileNameCharPointer);
			if (isFileExists(overrideMovieName)) {
				OutputDebugStream("オーバーライドファイルが存在するので、それを使う\n");
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

int pSSRExeJumpFromToOnSSRExeBGMFileName = 0x4B0291; // 関数はこのアドレスから、OnSSRExeBGMFileNameへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeBGMFileName = 0x4B0298; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeBGMFileName() {
	// スタックにためておく
	__asm {
		MOV ESI, DWORD PTR DS : [EAX * 4 + 0x929B20] // 元々の処理をここに
		mov iBGMFileNameCharPointer, ESI // ファイル名へのポインタを保存しておく
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

		MOV ESI, iBGMFileNameCharPointer; // ファイルポインタを上書きしているかもしれないのでESIに転写(上書きしてなければ元々EAXと同じ値が入っている)

		jmp pSSRExeReturnLblFromOnSSRExeBGMFileName
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromBGMFileName[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeBGMFileName() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeBGMFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeBGMFileName + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromBGMFileName + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeBGMFileName), cmdOnSSRExeJumpFromBGMFileName, 7, NULL);
	//7バイトのみ書き込む
	// 004B0291   8B3485 209B9200  MOV ESI,DWORD PTR DS:[EAX*4+929B20]　が 7バイトあるので綺麗に書き換えておく
}

#pragma managed
