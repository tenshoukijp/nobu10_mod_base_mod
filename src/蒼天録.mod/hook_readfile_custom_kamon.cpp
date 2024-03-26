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


// ��̉摜�͕���16, ������16�B�}�W�b�N�i���o�[�ɂȂ��Ă��܂����A����ύX�ɂȂ�����͉i�v�ɂ��Ȃ����߁A���̂܂ܖ��ߍ��ށB(�������̕����萔���g�����킩��₷��)


extern int nTargetKamonID;
BYTE kahouPalletteBuffer[KAMON_PALLETTE_SIZE];
bool isNextBufferKamonPallette = false;
BOOL Hook_ReadFileCustom_KamonGazou(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    // �܂��͉摜�f�[�^����
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
        OutputDebugStream("�Ɩ�摜�t�@�C���͑��݂��Ȃ�" + std::string(filenameBuf));
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());


    // ���Ƀp���b�g
    file.seekg(-(KAMON_PIC_WIDTH * KAMON_PIC_HIGHT) - KAMON_PALLETTE_SIZE, std::ios::end);

    std::vector<char> bufferPallette(KAMON_PALLETTE_SIZE);
    file.read(bufferPallette.data(), bufferPallette.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // �p���b�g�����T���Ă����B���ɓǂݍ��݂��������Ƀp���b�g�����R�s�[���邽�߁B
    memcpy(kahouPalletteBuffer, bufferPallette.data(), bufferPallette.size());
    isNextBufferKamonPallette = true;

    return TRUE;
}


extern int nTargetKamonGazouPalleteID;
BOOL Hook_ReadFileCustom_KamonGazouPallete(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    if (isNextBufferKamonPallette) {
        memcpy(lpBuffer, kahouPalletteBuffer, KAMON_PALLETTE_SIZE);
        isNextBufferKamonPallette = false;
    }

    return TRUE;
}


