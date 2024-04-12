/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その12

007EB92B  |. 8946 14        MOV DWORD PTR DS:[ESI+14],EAX
007EB92E  |> 8B4E 14        MOV ECX,DWORD PTR DS:[ESI+14]
007EB931  |. 8B56 10        MOV EDX,DWORD PTR DS:[ESI+10]
007EB934  |. 6A 01          PUSH 1
007EB936  |. 8D4424 08      LEA EAX,DWORD PTR SS:[ESP+8]
007EB93A  |. 50             PUSH EAX
007EB93B  |. 8B46 0C        MOV EAX,DWORD PTR DS:[ESI+C]
007EB93E  |. 51             PUSH ECX
007EB93F  |. 8B4E 08        MOV ECX,DWORD PTR DS:[ESI+8]
007EB942  |. 52             PUSH EDX
007EB943  |. 50             PUSH EAX
007EB944  |. C74424 18 0000>MOV DWORD PTR SS:[ESP+18],0
007EB94C  |. C74424 1C 0000>MOV DWORD PTR SS:[ESP+1C],0
007EB954  |. E8 0775FFFF    CALL Nobunaga.007E2E60
007EB959  |. 8B46 20        MOV EAX,DWORD PTR DS:[ESI+20]

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw12Execute() {
	OutputDebugStream("★★★★TextDraw12が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw12 = 0x7EB954; // 関数はこのアドレスから、OnSSRExeTextDraw12へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw12 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw12 = 0x7EB959; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw12() {
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

	OnSSRExeTextDraw12Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw12 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw12
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw12[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw12() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw12;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw12 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw12 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw12), cmdOnSSRExeJumpFromTextDraw12, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
