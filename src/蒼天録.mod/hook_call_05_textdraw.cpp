/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その５

00552C51  |. 8B5424 20      MOV EDX,DWORD PTR SS:[ESP+20]
00552C55  |. 83C4 0C        ADD ESP,0C
00552C58  |. 53             PUSH EBX
00552C59  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552C5D  |. 51             PUSH ECX
00552C5E  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C61  |. 6A 14          PUSH 14
00552C63  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C68  |. 52             PUSH EDX
00552C69  |. C74424 6C 1700>MOV DWORD PTR SS:[ESP+6C],17
00552C71  |. C74424 70 5100>MOV DWORD PTR SS:[ESP+70],51
00552C79  |. E8 E2012900    CALL Nobunaga.007E2E60
00552C7E  |. 8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+14]
00552C82  |. 53             PUSH EBX
00552C83  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552C87  |. 50             PUSH EAX
00552C88  |. 6A 14          PUSH 14

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw05Execute() {
	OutputDebugStream("★★★★TextDraw05が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw05 = 0x552C79; // 関数はこのアドレスから、OnSSRExeTextDraw05へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw05 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw05 = 0x552C7E; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw05() {
	// スタックにためておく
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

	OnSSRExeTextDraw05Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw05 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw05
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw05[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw05() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw05;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw05 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw05 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw05), cmdOnSSRExeJumpFromTextDraw05, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
