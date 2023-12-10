
/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その７

00552CA8  |. 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
00552CAC  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CAF  |. 53             PUSH EBX
00552CB0  |. 8D5424 5C      LEA EDX,DWORD PTR SS:[ESP+5C]
00552CB4  |. 52             PUSH EDX
00552CB5  |. 6A 14          PUSH 14
00552CB7  |. 68 FFFFFF00    PUSH 0FFFFFF
00552CBC  |. 50             PUSH EAX
00552CBD  |. C74424 6C 1700>MOV DWORD PTR SS:[ESP+6C],17
00552CC5  |. C74424 70 B101>MOV DWORD PTR SS:[ESP+70],1B1
00552CCD  |. E8 8E012900    CALL Nobunaga.007E2E60
00552CD2  |. 53             PUSH EBX
00552CD3  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552CD7  |. 51             PUSH ECX

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw07Execute() {
	OutputDebugStream("★★★★TextDraw07が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw07 = 0x552CCD; // 関数はこのアドレスから、OnSSRExeTextDraw07へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw07 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw07 = 0x552CD2; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw07() {
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

	OnSSRExeTextDraw07Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw07 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw07
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw07[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw07() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw07;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw07 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw07 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw07), cmdOnSSRExeJumpFromTextDraw07, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
