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


// ��̉摜�͕���96, ������120�B�}�W�b�N�i���o�[�ɂȂ��Ă��܂����A����ύX�ɂȂ�����͉i�v�ɂ��Ȃ����߁A���̂܂ܖ��ߍ��ށB(�������̕����萔���g�����킩��₷��)


extern int nTargetKahouGazouID;
BYTE kahouPalletteBuffer[KAHOU_PALLETTE_SIZE];
bool isNextBufferKahouPallette = false;
BOOL Hook_ReadFileCustom_KahouGazou(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    // �܂��͉摜�f�[�^����
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
        OutputDebugStream("�ƕ�摜�t�@�C���͑��݂��Ȃ�" + std::string(filenameBuf));
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());


    // ���Ƀp���b�g
    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT) - KAHOU_PALLETTE_SIZE, std::ios::end);

    std::vector<char> bufferPallette(KAHOU_PALLETTE_SIZE);
    file.read(bufferPallette.data(), bufferPallette.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // �p���b�g�����T���Ă����B���ɓǂݍ��݂��������Ƀp���b�g�����R�s�[���邽�߁B
    memcpy(kahouPalletteBuffer, bufferPallette.data(), bufferPallette.size());
    isNextBufferKahouPallette = true;

    return TRUE;
}


extern int nTargetKahouGazouPalleteID;
BOOL Hook_ReadFileCustom_KahouGazouPallete(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    if (isNextBufferKahouPallette) {
        memcpy(lpBuffer, kahouPalletteBuffer, KAHOU_PALLETTE_SIZE);
        isNextBufferKahouPallette = false;
    }

    return TRUE;
}


