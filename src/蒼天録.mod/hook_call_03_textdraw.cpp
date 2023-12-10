/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その３

00552BC7  |. 8B5424 10      MOV EDX,DWORD PTR SS:[ESP+10]
00552BCB  |. 53             PUSH EBX
00552BCC  |. 8D4C24 5C      LEA ECX,DWORD PTR SS:[ESP+5C]
00552BD0  |. 51             PUSH ECX
00552BD1  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552BD4  |. 6A 14          PUSH 14
00552BD6  |. 68 FFFFFF00    PUSH 0FFFFFF
00552BDB  |. 52             PUSH EDX
00552BDC  |. C74424 6C 1400>MOV DWORD PTR SS:[ESP+6C],14
00552BE4  |. C74424 70 6B01>MOV DWORD PTR SS:[ESP+70],16B
00552BEC  |. E8 6F022900    CALL Nobunaga.007E2E60
00552BF1  |. 8B4C24 10      MOV ECX,DWORD PTR SS:[ESP+10]
00552BF5  |. 53             PUSH EBX
00552BF6  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552BFA  |. 50             PUSH EAX
00552BFB  |. 6A 14          PUSH 14
*/

#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw03Execute() {
	OutputDebugStream("★★★★TextDraw03が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw03 = 0x552BEC; // 関数はこのアドレスから、OnSSRExeTextDraw03へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw03 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw03 = 0x552BF1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw03() {
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

	OnSSRExeTextDraw03Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw03 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw03
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw03[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw03() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw03;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw03 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw03 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw03), cmdOnSSRExeJumpFromTextDraw03, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
