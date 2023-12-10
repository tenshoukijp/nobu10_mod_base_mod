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
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

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

    // ���̉摜���R�s�[
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(lpBuffer, buffer.data(), buffer.size());
    }


    return TRUE;
}


extern int nTargetKaoPalleteID;
BOOL Hook_ReadFileCustom_BushouKaoPallette(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestBushouKaoID(nTargetKaoPalleteID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\DATA\\KAOCG\\%04d.bmp", nTargetKaoPalleteID);
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

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT)- KAO_PALLETTE_SIZE, std::ios::end);

    std::vector<char> buffer(KAO_PALLETTE_SIZE);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // ���̉摜���R�s�[
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(lpBuffer, buffer.data(), buffer.size());
    }


    return TRUE;
}


