/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その15

007F7328   . 8B77 44        MOV ESI,DWORD PTR DS:[EDI+44]
007F732B   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]
007F732E   . 74 22          JE SHORT Nobunaga.007F7352
007F7330   > 8B4E 18        MOV ECX,DWORD PTR DS:[ESI+18]
007F7333   . 8B56 14        MOV EDX,DWORD PTR DS:[ESI+14]
007F7336   . 8B46 10        MOV EAX,DWORD PTR DS:[ESI+10]
007F7339   . 6A 00          PUSH 0
007F733B   . 56             PUSH ESI
007F733C   . 51             PUSH ECX
007F733D   . 52             PUSH EDX
007F733E   . 50             PUSH EAX
007F733F   . 8BCF           MOV ECX,EDI
007F7341   . E8 1ABBFEFF    CALL Nobunaga.007E2E60
007F7346   . 85C0           TEST EAX,EAX
007F7348   . 7C 10          JL SHORT Nobunaga.007F735A
007F734A   . 8B76 20        MOV ESI,DWORD PTR DS:[ESI+20]
007F734D   . 3B77 4C        CMP ESI,DWORD PTR DS:[EDI+4C]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"


#pragma unmanaged


void OnSSRExeTextDraw15Execute() {
	OutputDebugStream("★★★★TextDraw15が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw15 = 0x7F7341; // 関数はこのアドレスから、OnSSRExeTextDraw15へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw15 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw15 = 0x7F7346; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw15() {
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

	OnSSRExeTextDraw15Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw15 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw15
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw15[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw15() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw15;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw15 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw15 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw15), cmdOnSSRExeJumpFromTextDraw15, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
