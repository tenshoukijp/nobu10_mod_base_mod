/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その４

00552BF1  |. 8B4C24 10      MOV ECX,DWORD PTR SS:[ESP+10]
00552BF5  |. 53             PUSH EBX
00552BF6  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552BFA  |. 50             PUSH EAX
00552BFB  |. 6A 14          PUSH 14
00552BFD  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C02  |. 51             PUSH ECX
00552C03  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C06  |. C74424 6C 1400>MOV DWORD PTR SS:[ESP+6C],14
00552C0E  |. C74424 70 1B02>MOV DWORD PTR SS:[ESP+70],21B
00552C16  |. E8 45022900    CALL Nobunaga.007E2E60
00552C1B  |. 8B17           MOV EDX,DWORD PTR DS:[EDI]
00552C1D  |. 6A FF          PUSH -1
00552C1F  |. 8D4424 14      LEA EAX,DWORD PTR SS:[ESP+14]
00552C23  |. 50             PUSH EAX
00552C24  |. 6A 2F          PUSH 2F

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw04Execute() {
	OutputDebugStream("★★★★TextDraw04が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw04 = 0x552C16; // 関数はこのアドレスから、OnSSRExeTextDraw04へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw04 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw04 = 0x552C1B; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw04() {
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

	OnSSRExeTextDraw04Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw04 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw04
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw04[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw04() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw04;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw04 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw04 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw04), cmdOnSSRExeJumpFromTextDraw04, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
