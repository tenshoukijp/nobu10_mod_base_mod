#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
#include "onigwrap.h"
using namespace std;

#pragma unmanaged

/*
この処理は、hook_call_13_textdraw の少し上。ループが始まる直前
007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
007EC1B7   895C24 3C        MOV DWORD PTR SS:[ESP+3C],EBX
007EC1BB   897424 14        MOV DWORD PTR SS:[ESP+14],ESI
007EC1BF   897424 20        MOV DWORD PTR SS:[ESP+20],ESI
007EC1C3   897424 28        MOV DWORD PTR SS:[ESP+28],ESI
007EC1C7   897424 18        MOV DWORD PTR SS:[ESP+18],ESI

*/

int TextDraw13Prev_ECX = 0;  // これがメッセージである
int TextDraw13Prev_ESI = 0;
int TextDraw13Prev_EBX = 0;

char bufTextDraw13Prev[512] = "これあれなんか？\xAもしかして置き換えなんか？\xAほんまなんか？ そうなんか？";

void OnSSRExeTextDraw13PrevExecute() {
	if (TextDraw13Prev_ECX != NULL && strlen((char*)TextDraw13Prev_ECX) > 0) {
		OutputDebugStream("■TextDraw13Prevが来たよ!!!★\n");
		OutputDebugStream("■TextDraw13Prev_ECX %x★\n", TextDraw13Prev_ECX);
		OutputDebugStream("■TextDraw13Prev_ECX STR %s★\n", TextDraw13Prev_ECX);
		OutputDebugStream("■TextDraw13Prev_ESI %x★\n", TextDraw13Prev_ESI);
		OutputDebugStream("■TextDraw13Prev_EBX %x★\n", TextDraw13Prev_EBX);
		/*
		if (OnigMatch((char*)TextDraw13Prev_ECX, "にご命令を")) {
			strcpy((char*)TextDraw13Prev_ECX, "これあれなんか？\xA置き換えたんか\xAほんまなんか？");
		}
		*/
	}
}


/*
007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
007EC1B7   895C24 3C        MOV DWORD PTR SS:[ESP+3C],EBX
007EC1BB   897424 14        MOV DWORD PTR SS:[ESP+14],ESI
007EC1BF   897424 20        MOV DWORD PTR SS:[ESP+20],ESI
007EC1C3   897424 28        MOV DWORD PTR SS:[ESP+28],ESI
007EC1C7   897424 18        MOV DWORD PTR SS:[ESP+18],ESI

*/

int pSSRExeJumpFromToOnSSRExeTextDraw13Prev = 0x7EC1AF; // 関数はこのアドレスから、OnSSRExeTextDraw13Prevへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeTextDraw13Prev = 0x7EC1B7; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13Prev() {
	// スタックにためておく
	__asm {

		mov TextDraw13Prev_ECX, ecx
		mov TextDraw13Prev_ESI, esi
		mov TextDraw13Prev_EBX, ebx

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDraw13PrevExecute();

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

		mov ecx, TextDraw13Prev_ECX

		MOV DWORD PTR SS : [ESP + 0x10] , ECX // 元の記述をここに
		MOV DWORD PTR SS : [ESP + 0x38] , ESI // 元の記述をここに

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13Prev
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw13Prev[9] = "\xE9\x90\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw13Prev() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw13Prev;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13Prev + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw13Prev + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13Prev), cmdOnSSRExeJumpFromTextDraw13Prev, 8, NULL);
	/*
	8バイトのみ書き込む
	007EC1AF   894C24 10        MOV DWORD PTR SS:[ESP+10],ECX
	007EC1B3   897424 38        MOV DWORD PTR SS:[ESP+38],ESI
	の２つをすり潰すので8バイトかいておく
	*/
}

#pragma managed
