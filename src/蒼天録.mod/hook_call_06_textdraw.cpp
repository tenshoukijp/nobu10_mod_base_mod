/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その６

00552C7E  |. 8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+14]
00552C82  |. 53             PUSH EBX
00552C83  |. 8D4424 5C      LEA EAX,DWORD PTR SS:[ESP+5C]
00552C87  |. 50             PUSH EAX
00552C88  |. 6A 14          PUSH 14
00552C8A  |. 68 FFFFFF00    PUSH 0FFFFFF
00552C8F  |. 51             PUSH ECX
00552C90  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552C93  |. C74424 6C 1900>MOV DWORD PTR SS:[ESP+6C],19
00552C9B  |. C74424 70 0301>MOV DWORD PTR SS:[ESP+70],103
00552CA3  |. E8 B8012900    CALL Nobunaga.007E2E60
00552CA8  |. 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
00552CAC  |. 8B4E 04        MOV ECX,DWORD PTR DS:[ESI+4]
00552CAF  |. 53             PUSH EBX
00552CB0  |. 8D5424 5C      LEA EDX,DWORD PTR SS:[ESP+5C]

*/


#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw06Execute() {
	OutputDebugStream("★★★★TextDraw06が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw06 = 0x552CA3; // 関数はこのアドレスから、OnSSRExeTextDraw06へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw06 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw06 = 0x552CA8; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw06() {
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

	OnSSRExeTextDraw06Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw06 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw06
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw06[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw06() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw06;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw06 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw06 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw06), cmdOnSSRExeJumpFromTextDraw06, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
