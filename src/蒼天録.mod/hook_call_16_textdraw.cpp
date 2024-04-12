/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その16

007F73C9   . 6A 00          PUSH 0
007F73CB   . 56             PUSH ESI
007F73CC   . 51             PUSH ECX
007F73CD   . 52             PUSH EDX
007F73CE   . 50             PUSH EAX
007F73CF   . 8BCF           MOV ECX,EDI
007F73D1   . E8 8ABAFEFF    CALL Nobunaga.007E2E60
007F73D6   . 85C0           TEST EAX,EAX
007F73D8   . 7C 10          JL SHORT Nobunaga.007F73EA
007F73DA   . 8B76 20        MOV ESI,DWORD PTR DS:[ESI+20]
007F73DD   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw16Execute() {
	OutputDebugStream("★★★★TextDraw16が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw16 = 0x7F73D1; // 関数はこのアドレスから、OnSSRExeTextDraw16へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw16 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw16 = 0x7F73D6; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw16() {
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

	OnSSRExeTextDraw16Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw16 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw16
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw16[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw16() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw16;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw16 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw16 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw16), cmdOnSSRExeJumpFromTextDraw16, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
