/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その１

00552B73   8B4C24 10        MOV ECX,DWORD PTR SS:[ESP+10]
00552B77   53               PUSH EBX
00552B78   8D4424 5C        LEA EAX,DWORD PTR SS:[ESP+5C]
00552B7C   50               PUSH EAX
00552B7D   6A 14            PUSH 14
00552B7F   68 FFFFFF00      PUSH 0FFFFFF
00552B84   51               PUSH ECX
00552B85   8B4E 04          MOV ECX,DWORD PTR DS:[ESI+4]
00552B88   C74424 6C 140000>MOV DWORD PTR SS:[ESP+6C],14
00552B90   C74424 70 0B0000>MOV DWORD PTR SS:[ESP+70],0B
00552B98   E8 C3022900      CALL Nobunaga.007E2E60
00552B9D   8B4424 10        MOV EAX,DWORD PTR SS:[ESP+10]
00552BA1   8B4E 04          MOV ECX,DWORD PTR DS:[ESI+4]
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw01Execute() {
	OutputDebugStream("★★★★TextDraw01が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw01 = 0x552B98; // 関数はこのアドレスから、OnSSRExeTextDraw01へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw01 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw01 = 0x552B9D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw01() {
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

	OnSSRExeTextDraw01Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw01 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw01
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw01[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw01() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw01;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw01 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw01 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw01), cmdOnSSRExeJumpFromTextDraw01, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
