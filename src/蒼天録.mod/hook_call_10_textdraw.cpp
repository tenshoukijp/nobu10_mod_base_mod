/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その10

007E3119   > 8B4F 10        MOV ECX,DWORD PTR DS:[EDI+10]
007E311C   . 8B57 0C        MOV EDX,DWORD PTR DS:[EDI+C]
007E311F   . 6A 00          PUSH 0
007E3121   . 8D47 14        LEA EAX,DWORD PTR DS:[EDI+14]
007E3124   . 50             PUSH EAX
007E3125   . 8B47 08        MOV EAX,DWORD PTR DS:[EDI+8]
007E3128   . 51             PUSH ECX
007E3129   . 8B4F 04        MOV ECX,DWORD PTR DS:[EDI+4]
007E312C   . 52             PUSH EDX
007E312D   . 50             PUSH EAX
007E312E   . E8 2DFDFFFF    CALL Nobunaga.007E2E60
007E3133   . 5F             POP EDI
007E3134   . 5E             POP ESI
007E3135   . 83C4 10        ADD ESP,10

*/

#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw10Execute() {
	OutputDebugStream("★★★★TextDraw10が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw10 = 0x7E312E; // 関数はこのアドレスから、OnSSRExeTextDraw10へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw10 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw10 = 0x7E3133; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw10() {
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

	OnSSRExeTextDraw10Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw10 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw10
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw10[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw10() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw10;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw10 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw10 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw10), cmdOnSSRExeJumpFromTextDraw10, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
