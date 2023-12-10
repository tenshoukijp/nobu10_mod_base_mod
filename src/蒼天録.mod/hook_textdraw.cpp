#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged

/*
文字列表示のための基本関数だとおもわれる
007E2E60  /$ 6A FF          PUSH -1
007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
007E2E67  |. 64:A1 00000000 MOV EAX,DWORD PTR FS:[0]
007E2E6D  |. 50             PUSH EAX
007E2E6E  |. 64:8925 000000>MOV DWORD PTR FS:[0],ESP
007E2E75  |. 83EC 10        SUB ESP,10
007E2E78  |. 8B4424 28      MOV EAX,DWORD PTR SS:[ESP+28]
007E2E7C  |. 55             PUSH EBP
007E2E7D  |. 57             PUSH EDI
007E2E7E  |. 8BF9           MOV EDI,ECX
007E2E80  |. 8B0D 60D8AC00  MOV ECX,DWORD PTR DS:[ACD860]
007E2E86  |. 50             PUSH EAX
007E2E87  |. 897C24 14      MOV DWORD PTR SS:[ESP+14],EDI
007E2E8B  |. E8 40CDFEFF    CALL Nobunaga.007CFBD0
007E2E90  |. 33ED           XOR EBP,EBP
007E2E92  |. 3BC5           CMP EAX,EBP


上の関数を使っているところは下記
参照データ ： Nobunaga:.text -> 007E2E60
ｱﾄﾞﾚｽ      ﾃﾞｨｽｱｾﾝﾌﾞﾙ                                ｺﾒﾝﾄ
00552B98   CALL Nobunaga.007E2E60
00552BC2   CALL Nobunaga.007E2E60
00552BEC   CALL Nobunaga.007E2E60
00552C16   CALL Nobunaga.007E2E60
00552C79   CALL Nobunaga.007E2E60
00552CA3   CALL Nobunaga.007E2E60
00552CCD   CALL Nobunaga.007E2E60
00552CF7   CALL Nobunaga.007E2E60
007D5141   CALL Nobunaga.007E2E60
007E312E   CALL Nobunaga.007E2E60
007E31BA   CALL Nobunaga.007E2E60
007EB954   CALL Nobunaga.007E2E60
007EC474   CALL Nobunaga.007E2E60
007F7078   CALL Nobunaga.007E2E60
007F7341   CALL Nobunaga.007E2E60
007F73D1   CALL Nobunaga.007E2E60


// このような５つの引数の使い方をしている。
007F73C9   . 6A 00          PUSH 0                                                    ; /Arg5 = 00000000
007F73CB   . 56             PUSH ESI                                                  ; |Arg4
007F73CC   . 51             PUSH ECX                                                  ; |Arg3
007F73CD   . 52             PUSH EDX                                                  ; |Arg2
007F73CE   . 50             PUSH EAX                                                  ; |Arg1
007F73D1   . E8 8ABAFEFF    CALL Nobunaga.007E2E60                                    ; \Nobunaga.007E2E60



*/




int nTextDrawEaxBackup = 0;
int nTextDrawArg1 = 0;
int nTextDrawArg2 = 0;
int nTextDrawArgTargetString = 0; // これが文字列のアドレス
int nTextDrawArg4 = 0;
int nTextDrawArg5 = 0;

void OnSSRExeTextDrawExecute() {
	if (nTextDrawArgTargetString != NULL && strlen((char *)nTextDrawArgTargetString) > 0) {
		OutputDebugStream("TextDrawが通りますよ");
		OutputDebugStream("nTextDrawArg1%x\n", nTextDrawArg1);
		OutputDebugStream("nTextDrawArg2%x\n", nTextDrawArg2);
		OutputDebugStream("nTextDrawArg3%x\n", nTextDrawArgTargetString);
		OutputDebugStream("★nTextDrawArgSTR%s\n", nTextDrawArgTargetString);
		OutputDebugStream("nTextDrawArg4%x\n", nTextDrawArg4);
		OutputDebugStream("nTextDrawArg5%x\n", nTextDrawArg5);
	}
}


/*
007E2E60  /$ 6A FF          PUSH -1
007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
007E2E67  |. 64:A1 00000000 MOV EAX,DWORD PTR FS:[0]

*/

int pSSRExeJumpFromToOnSSRExeTextDraw = 0x7E2E60; // 関数はこのアドレスから、OnSSRExeTextDrawへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeTextDraw = 0x7E2E67; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeTextDraw() {
	// スタックにためておく
	__asm {
		push -1          // 元々あった記述をここで
		PUSH 0x8364F8    // 元々あった記述をここで

		mov nTextDrawEaxBackup, eax          // 後で復元するため、現状のEAXは取っておく

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 5番目の引数を取得
		mov nTextDrawArg5, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 4番目の引数を取得
		mov nTextDrawArg4, eax
		MOV eax, DWORD PTR SS : [ESP + 0xC] // 3番目の引数を取得
		mov nTextDrawArgTargetString, eax
		MOV eax, DWORD PTR SS : [ESP + 0x10] // 2番目の引数を取得
		mov nTextDrawArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0x14] // 1番目の引数を取得
		mov nTextDrawArg1, eax

		mov eax, nTextDrawEaxBackup		     // EAXを復元する



		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeTextDrawExecute();

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

		jmp pSSRExeReturnLblFromOnSSRExeTextDraw
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromTextDraw[8] = "\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeTextDraw() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeTextDraw;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeTextDraw + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromTextDraw + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeTextDraw), cmdOnSSRExeJumpFromTextDraw, 7, NULL);
	//7バイトのみ書き込む
	// 007E2E60  /$ 6A FF          PUSH -1
	// 007E2E62  |. 68 F8648300    PUSH Nobunaga.008364F8                                    ;  Entry address; 構造化例外ﾊﾝﾄﾞﾗのｲﾝｽﾄｰﾙ
	// の２つをすり潰すので７バイトかいておく
}

#pragma managed
