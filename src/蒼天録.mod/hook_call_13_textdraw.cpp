/*
概ね画面下部中央にでる「会話」は、この13番が担当している模様。
(★forループの中に処理が入っている可能性がきわめて高い)
*/


/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その13

007EC44A  |> 897C24 20      |MOV DWORD PTR SS:[ESP+20],EDI
007EC44E  |. 897C24 18      |MOV DWORD PTR SS:[ESP+18],EDI
007EC452  |. EB 25          |JMP SHORT Nobunaga.007EC479
007EC454  |> 57             |PUSH EDI
007EC455  |. 8D4424 44      |LEA EAX,DWORD PTR SS:[ESP+44]
007EC459  |. 50             |PUSH EAX
007EC45A  |. 8B4424 24      |MOV EAX,DWORD PTR SS:[ESP+24]
007EC45E  |. 8B88 80000000  |MOV ECX,DWORD PTR DS:[EAX+80]
007EC464  |. 8B50 70        |MOV EDX,DWORD PTR DS:[EAX+70]
007EC467  |. 8B8424 9000000>|MOV EAX,DWORD PTR SS:[ESP+90]
007EC46E  |. 51             |PUSH ECX
007EC46F  |. 8B48 04        |MOV ECX,DWORD PTR DS:[EAX+4]
007EC472  |. 52             |PUSH EDX
007EC473  |. 53             |PUSH EBX
007EC474  |. E8 E769FFFF    |CALL Nobunaga.007E2E60
007EC479  |> 53             |PUSH EBX

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw13Execute() {
	// OutputDebugStream("★★★★TextDraw13が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw13 = 0x7EC474; // 関数はこのアドレスから、OnSSRExeTextDraw13へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw13 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw13 = 0x7EC479; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw13() {
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

	OnSSRExeTextDraw13Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw13 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw13
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw13[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw13() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw13;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw13 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw13 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw13), cmdOnSSRExeJumpFromTextDraw13, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
