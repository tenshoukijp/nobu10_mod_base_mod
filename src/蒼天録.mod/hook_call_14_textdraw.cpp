/*
概ね通常の文字列はほとんど「この14番」から表示されている。
ただし、列伝もここ経由で表示されているので油断がならない。
*/

/*
0x07E2E60 = textdraw のラップ関数を呼び出している場所、その14

007F7049  |. 8970 20        MOV DWORD PTR DS:[EAX+20],ESI
007F704C  |> C746 20 000000>MOV DWORD PTR DS:[ESI+20],0
007F7053  |. 8975 48        MOV DWORD PTR SS:[EBP+48],ESI
007F7056  |> 8B46 1C        MOV EAX,DWORD PTR DS:[ESI+1C]
007F7059  |. 85C0           TEST EAX,EAX
007F705B  |. 75 03          JNZ SHORT Nobunaga.007F7060
007F705D  |. 8975 44        MOV DWORD PTR SS:[EBP+44],ESI
007F7060  |> 8B4C24 28      MOV ECX,DWORD PTR SS:[ESP+28]
007F7064  |. 8B5424 24      MOV EDX,DWORD PTR SS:[ESP+24]
007F7068  |. 6A 00          PUSH 0
007F706A  |. 8D4424 18      LEA EAX,DWORD PTR SS:[ESP+18]
007F706E  |. 50             PUSH EAX
007F706F  |. 8B4424 28      MOV EAX,DWORD PTR SS:[ESP+28]
007F7073  |. 51             PUSH ECX
007F7074  |. 52             PUSH EDX
007F7075  |. 50             PUSH EAX
007F7076  |. 8BCD           MOV ECX,EBP
007F7078  |. E8 E3BDFEFF    CALL Nobunaga.007E2E60
007F707D  |. 85C0           TEST EAX,EAX

*/



#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

#pragma unmanaged


void OnSSRExeTextDraw14Execute() {
	// OutputDebugStream("★★★★TextDraw14が来たよ!!!★\n");
}

int pSSRExeJumpFromToOnSSRExeTextDraw14 = 0x7F7078; // 関数はこのアドレスから、OnSSRExeTextDraw14へとジャンプしてくる。
int pSSRExeJumpCallFromToOnSSRExeTextDraw14 = 0x7E2E60; // 元々あった処理のCall先
int pSSRExeReturnLblFromOnSSRExeTextDraw14 = 0x7F707D; // 関数が最後までいくと、この実行exe内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw14() {
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

	OnSSRExeTextDraw14Execute();

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

		call pSSRExeJumpCallFromToOnSSRExeTextDraw14 // 元の処理

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw14
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw14[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw14() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw14;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw14 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw14 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令を実行exeのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw14), cmdOnSSRExeJumpFromTextDraw14, 5, NULL); //5バイトのみ書き込む
}

#pragma managed
