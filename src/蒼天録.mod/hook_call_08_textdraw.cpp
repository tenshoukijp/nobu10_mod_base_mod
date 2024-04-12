/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その８

00552CD2  |. 53             PUSH EBX
00552CD3  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552CD7  |. 51             PUSH ECX
00552CD8  |. 6A 14          PUSH 14
00552CDA  |. C74424 64 1700>MOV DWORD PTR SS:[ESP+64],17
00552CE2  |. C74424 68 6102>MOV DWORD PTR SS:[ESP+68],261
00552CEA  |. 68 FFFFFF00    PUSH 0FFFFFF
00552CEF  |. 8B5424 24      MOV EDX,DWORD PTR SS:[ESP+24]
00552CF3  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CF6  |. 52             PUSH EDX
00552CF7  |. E8 64012900    CALL Nobunaga.007E2E60
00552CFC  |. 8B45 0C        MOV EAX,DWORD PTR SS:[EBP+C]
00552CFF  |. 50             PUSH EAX
00552D00  |. 8D4C24 34      LEA ECX,DWORD PTR SS:[ESP+34]
00552D04  |. 51             PUSH ECX

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw08Execute() {
	OutputDebugStream("★★★★TextDraw08が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw08 = 0x552CF7; // 関数はこのアドレスから、OnSSRExeTextDraw08へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw08 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw08 = 0x552CFC; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw08() {
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

	OnSSRExeTextDraw08Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw08 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw08
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw08[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw08() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw08;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw08 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw08 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw08), cmdOnSSRExeJumpFromTextDraw08, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
