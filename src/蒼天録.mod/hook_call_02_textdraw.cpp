/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その２

00552BA4   53               PUSH EBX
00552BA5   8D5424 5C        LEA EDX,DWORD PTR SS:[ESP+5C]
00552BA9   52               PUSH EDX
00552BAA   6A 14            PUSH 14
00552BAC   68 FFFFFF00      PUSH 0FFFFFF
00552BB1   C74424 68 160000>MOV DWORD PTR SS:[ESP+68],16
00552BB9   C74424 6C BD0000>MOV DWORD PTR SS:[ESP+6C],0BD
00552BC1   50               PUSH EAX
00552BC2   E8 99022900      CALL Nobunaga.007E2E60
00552BC7   8B5424 10        MOV EDX,DWORD PTR SS:[ESP+10]
00552BCB   53               PUSH EBX
00552BCC   8D4C24 5C        LEA ECX,DWORD PTR SS:[ESP+5C]
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw02Execute() {
	OutputDebugStream("★★★★TextDraw02が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw02 = 0x552BC2; // 関数はこのアドレスから、OnSSRExeTextDraw02へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw02 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw02 = 0x552BC7; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw02() {
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

	OnSSRExeTextDraw02Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw02 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw02
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw02[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw02() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw02;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw02 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw02 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw02), cmdOnSSRExeJumpFromTextDraw02, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
