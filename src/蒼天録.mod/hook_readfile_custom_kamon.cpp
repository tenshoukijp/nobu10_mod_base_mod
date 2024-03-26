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


// 顔の画像は幅が16, 高さが16。マジックナンバーになってしまうが、今後変更になったりは永久にしないため、そのまま埋め込む。(そっちの方が定数名使うよりわかりやすい)


extern int nTargetKamonID;
BYTE kahouPalletteBuffer[KAMON_PALLETTE_SIZE];
bool isNextBufferKamonPallette = false;
BOOL Hook_ReadFileCustom_KamonGazou(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    // まずは画像データから
    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestKamonPicID(nTargetKamonID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\DATA\\KAMON\\%03d.bmp", nTargetKamonID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        OutputDebugStream("家紋画像ファイルは存在しない" + std::string(filenameBuf));
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("ファイル" + filename + "を開くことができませんでした。\n");
        return FALSE;
    }

    file.seekg(-(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());


    // 次にパレット
    file.seekg(-(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT) - KAMON_PALLETTE_SIZE, std::ios::end);

    std::vector<char> bufferPallette(KAMON_PALLETTE_SIZE);
    file.read(bufferPallette.data(), bufferPallette.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    // パレット情報を控えておく。次に読み込みが来た時にパレット情報をコピーするため。
    memcpy(kahouPalletteBuffer, bufferPallette.data(), bufferPallette.size());
    isNextBufferKamonPallette = true;

    return TRUE;
}


extern int nTargetKamonGazouPalleteID;
BOOL Hook_ReadFileCustom_KamonGazouPallete(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    if (isNextBufferKamonPallette) {
        memcpy(lpBuffer, kahouPalletteBuffer, KAMON_PALLETTE_SIZE);
        isNextBufferKamonPallette = false;
    }

    return TRUE;
}


