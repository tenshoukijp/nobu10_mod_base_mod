/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その９
(★forループの中に処理が入っている可能性がきわめて高い)

007D511A  |. 8B45 04        ||MOV EAX,DWORD PTR SS:[EBP+4]
007D511D  |. 2B4424 28      ||SUB EAX,DWORD PTR SS:[ESP+28]
007D5121  |. 99             ||CDQ
007D5122  |. 2BC2           ||SUB EAX,EDX
007D5124  |. D1F8           ||SAR EAX,1
007D5126  |> 014424 20      ||ADD DWORD PTR SS:[ESP+20],EAX
007D512A  |> 8B46 10        ||MOV EAX,DWORD PTR DS:[ESI+10]
007D512D  |. 8B4E 0C        ||MOV ECX,DWORD PTR DS:[ESI+C]
007D5130  |. 8B56 08        ||MOV EDX,DWORD PTR DS:[ESI+8]
007D5133  |. 6A 00          ||PUSH 0
007D5135  |. 8D7C24 20      ||LEA EDI,DWORD PTR SS:[ESP+20]
007D5139  |. 57             ||PUSH EDI
007D513A  |. 50             ||PUSH EAX
007D513B  |. 51             ||PUSH ECX
007D513C  |. 8B4C24 68      ||MOV ECX,DWORD PTR SS:[ESP+68]
007D5140  |. 52             ||PUSH EDX
007D5141  |. E8 1ADD0000    ||CALL Nobunaga.007E2E60
007D5146  |. 8B5C24 14      ||MOV EBX,DWORD PTR SS:[ESP+14]
007D514A  |. 8B4424 60      ||MOV EAX,DWORD PTR SS:[ESP+60]
007D514E  |. 83C6 18        ||ADD ESI,18
*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw09Execute() {
	OutputDebugStream("★★★★TextDraw09が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw09 = 0x7D5141; // 関数はこのアドレスから、OnSSRExeTextDraw09へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw09 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw09 = 0x7D5146; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw09() {
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

	OnSSRExeTextDraw09Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw09 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw09
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw09[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw09() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw09;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw09 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw09 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw09), cmdOnSSRExeJumpFromTextDraw09, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
