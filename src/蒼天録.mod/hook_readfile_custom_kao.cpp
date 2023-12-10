#include <Windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "hook_readfile_custom.h"
#include "output_debug_stream.h"
#include "javascript_mod.h"
#include "file_attribute.h"


// ��̉摜�͕���96, ������120�B�}�W�b�N�i���o�[�ɂȂ��Ă��܂����A����ύX�ɂȂ�����͉i�v�ɂ��Ȃ����߁A���̂܂ܖ��ߍ��ށB(�������̕����萔���g�����킩��₷��)


extern int nTargetKaoID;
BYTE kaoPalletteBuffer[KAO_PALLETTE_SIZE];
bool isNextBufferKaoPallette = false;
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    // �܂��͉摜�f�[�^����
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
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAO_PIC_WIDTH * KAO_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    memcpy(lpBuffer, buffer.data(), buffer.size());

    // ���Ƀp���b�g
    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT) - KAO_PALLETTE_SIZE, std::ios::end);

    std::vector<char> palBuffer(KAO_PALLETTE_SIZE);
    file.read(palBuffer.data(), palBuffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // �p���b�g�����T���Ă����B���ɓǂݍ��݂��������Ƀp���b�g�����R�s�[���邽�߁B
    memcpy(kaoPalletteBuffer, palBuffer.data(), palBuffer.size());
    isNextBufferKaoPallette = true;

    return TRUE;
}


BOOL Hook_ReadFileCustom_BushouKaoPallette(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {
    if (isNextBufferKaoPallette) {
        memcpy(lpBuffer, kaoPalletteBuffer, KAO_PALLETTE_SIZE);
        isNextBufferKaoPallette = false;
    }


    return TRUE;
}


