/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その11

007E319F  |. 8B46 08        MOV EAX,DWORD PTR DS:[ESI+8]
007E31A2  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
007E31A5  |. 53             PUSH EBX
007E31A6  |. 8D5424 10      LEA EDX,DWORD PTR SS:[ESP+10]
007E31AA  |. 52             PUSH EDX
007E31AB  |. 8B16           MOV EDX,DWORD PTR DS:[ESI]
007E31AD  |. 50             PUSH EAX
007E31AE  |. 51             PUSH ECX
007E31AF  |. 52             PUSH EDX
007E31B0  |. 8BCF           MOV ECX,EDI
007E31B2  |. 895C24 20      MOV DWORD PTR SS:[ESP+20],EBX
007E31B6  |. 895C24 24      MOV DWORD PTR SS:[ESP+24],EBX
007E31BA  |. E8 A1FCFFFF    CALL Nobunaga.007E2E60

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw11Execute() {
	OutputDebugStream("★★★★TextDraw11が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw11 = 0x7F7078; // 関数はこのアドレスから、OnSSRExeTextDraw11へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw11 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw11 = 0x7E31BF; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw11() {
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

	OnSSRExeTextDraw11Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw11 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw11
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw11[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw11() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw11;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw11 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw11 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw11), cmdOnSSRExeJumpFromTextDraw11, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
