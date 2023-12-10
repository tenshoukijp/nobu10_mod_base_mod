#include <Windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "hook_readfile_custom.h"
#include "output_debug_stream.h"
#include "javascript_mod.h"
#include "file_attribute.h"


// 顔の画像は幅が96, 高さが120。マジックナンバーになってしまうが、今後変更になったりは永久にしないため、そのまま埋め込む。(そっちの方が定数名使うよりわかりやすい)


extern int nTargetKaoID;
BYTE kaoPalletteBuffer[KAO_PALLETTE_SIZE];
bool isNextBufferKaoPallette = false;
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    // まずは画像データから
    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestBushouKaoID(nTargetKaoID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\DATA\\KAOCG\\%04d.bmp", nTargetKaoID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("ファイル" + filename + "を開くことができませんでした。\n");
        return FALSE;
    }

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAO_PIC_WIDTH * KAO_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());

    // 次にパレット
    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT) - KAO_PALLETTE_SIZE, std::ios::end);

    std::vector<char> palBuffer(KAO_PALLETTE_SIZE);
    file.read(palBuffer.data(), palBuffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    // パレット情報を控えておく。次に読み込みが来た時にパレット情報をコピーするため。
    memcpy(kaoPalletteBuffer, palBuffer.data(), palBuffer.size());
    isNextBufferKaoPallette = true;

    return TRUE;
}


BOOL Hook_ReadFileCustom_BushouKaoPallette(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {
    if (isNextBufferKaoPallette) {
        memcpy(lpBuffer, kaoPalletteBuffer, KAO_PALLETTE_SIZE);
        isNextBufferKaoPallette = false;
    }


    return TRUE;
}


