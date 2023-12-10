#include <Windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>

#include "hook_readfile_custom.h"
#include "output_debug_stream.h"
#include "javascript_mod.h"
#include "file_attribute.h"


// 顔の画像は幅が96, 高さが120。マジックナンバーになってしまうが、今後変更になったりは永久にしないため、そのまま埋め込む。(そっちの方が定数名使うよりわかりやすい)


extern int nTargetKahouGazouID;
BYTE kahouPalletteBuffer[KAHOU_PALLETTE_SIZE];
bool isNextBufferKahouPallette = false;
BOOL Hook_ReadFileCustom_KahouGazou(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    // まずは画像データから
    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestKahouPicID(nTargetKahouGazouID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\DATA\\KAHOU\\%03d.bmp", nTargetKahouGazouID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        OutputDebugStream("家宝画像ファイルは存在しない" + std::string(filenameBuf));
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("ファイル" + filename + "を開くことができませんでした。\n");
        return FALSE;
    }

    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());


    // 次にパレット
    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT) - KAHOU_PALLETTE_SIZE, std::ios::end);

    std::vector<char> bufferPallette(KAHOU_PALLETTE_SIZE);
    file.read(bufferPallette.data(), bufferPallette.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    // パレット情報を控えておく。次に読み込みが来た時にパレット情報をコピーするため。
    memcpy(kahouPalletteBuffer, bufferPallette.data(), bufferPallette.size());
    isNextBufferKahouPallette = true;

    return TRUE;
}


extern int nTargetKahouGazouPalleteID;
BOOL Hook_ReadFileCustom_KahouGazouPallete(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    if (isNextBufferKahouPallette) {
        memcpy(lpBuffer, kahouPalletteBuffer, KAHOU_PALLETTE_SIZE);
        isNextBufferKahouPallette = false;
    }

    return TRUE;
}


