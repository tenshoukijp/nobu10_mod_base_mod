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
007EC142   68 FB698300      PUSH Nobunaga.008369FB                   ; ASCII "ｸTｷ・; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
007EC147   64:A1 00000000   MOV EAX,DWORD PTR FS:[0]
007EC14D   50               PUSH EAX
007EC14E   64:8925 00000000 MOV DWORD PTR FS:[0],ESP
007EC155   83EC 68          SUB ESP,68


呼び出し元
007EC5AE   53               PUSH EBX
007EC5AF   8D4C24 14        LEA ECX,DWORD PTR SS:[ESP+14]
007EC5B3   51               PUSH ECX
007EC5B4   8BCE             MOV ECX,ESI
007EC5B6   E8 85FBFFFF      CALL Nobunaga.007EC140
007EC5BB   6A 0C            PUSH 0C

呼び出し元
007EC71E   6A 00            PUSH 0
007EC720   50               PUSH EAX
007EC721   C74424 1C 000000>MOV DWORD PTR SS:[ESP+1C],0
007EC729   E8 12FAFFFF      CALL Nobunaga.007EC140
007EC72E   8D4C24 04        LEA ECX,DWORD PTR SS:[ESP+4]


以上から引数は２つと思われる。

*/




int nTextDraw13WrapEaxBackup = 0;
int nTextDraw13WrapArg1 = 0;
int nTextDraw13WrapArg2 = 0;

void OnSSRExeTextDraw13WrapExecute() {
	OutputDebugStream("★★★★TextDraw13Wrapが来たよ!!!★\n");
	/*
	OutputDebugStream("★★★★nTextDraw13WrapArg1 %x★\n", nTextDraw13WrapArg1);
	OutputDebugStream("★★★★nTextDraw13WrapArg2 %x★\n", nTextDraw13WrapArg2);
	*/
}


/*
007EC140   6A FF            PUSH -1
007EC142   68 FB698300      PUSH Nobunaga.008369FB                   ; ASCII "ｸTｷ・; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
007EC147   64:A1 00000000   MOV EAX,DWORD PTR FS:[0]

*/

int pSSRExeJumpFromToOnSSRExeTextDraw13Wrap = 0x7EC140; // 関数はこのアドレスから、OnSSRExeTextDraw13Wrapへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeTextDraw13Wrap = 0x7EC147; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13Wrap() {
	// スタックにためておく
	__asm {
		push - 1          // 元々あった記述をここで
		PUSH 0x8369FB    // 元々あった記述をここで

		mov nTextDraw13WrapEaxBackup, eax          // 後で復元するため、現状のEAXは取っておく

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 5番目の引数を取得
		mov nTextDraw13WrapArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 4番目の引数を取得
		mov nTextDraw13WrapArg1, eax

		mov eax, nTextDraw13WrapEaxBackup		     // EAXを復元する

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

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13Wrap
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw13Wrap[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw13Wrap() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw13Wrap;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13Wrap + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw13Wrap + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13Wrap), cmdOnSSRExeJumpFromTextDraw13Wrap, 7, NULL);
	//7バイトのみ書き込む
	// 007E2E60  /$ 6A FF          PUSH -1
	// 007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
	// の２つをすり潰すので７バイトかいておく
}

#pragma managed
