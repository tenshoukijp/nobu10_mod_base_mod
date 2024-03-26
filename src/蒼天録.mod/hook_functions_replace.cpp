#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "output_debug_stream.h"
#include "game_font.h"
#include "game_process.h"
#include "game_window.h"
// #include "onigwrap.h"
// #include "on_event.h"
// #include "hook_textouta_custom.h"

#include "javascript_mod.h"
#include "file_attribute.h"
#include "hook_readfile_custom.h"

// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")

using namespace std;

// �ЂƂ̃��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInOneMod(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew,
    HMODULE hmodCaller)
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while (pImportDesc->Name) {
        PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0)
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
        ((PBYTE)hmodCaller + pImportDesc->FirstThunk);

    while (pThunk->u1.Function) {
        PROC* ppfn = (PROC*)&pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                hCurrentProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// ���ׂẴ��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInAllMods(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew)
{
    // ���W���[�����X�g���擾
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // ���ꂼ��̃��W���[���ɑ΂���ReplaceIATEntryInOneMod�����s
    while (bModuleResult) {
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}


//---------------------------WindowProcA

// �t�b�N����֐��̃v���g�^�C�v���`
using PFNDEFWINDOWPROCA = LRESULT(WINAPI *)(HWND, UINT, WPARAM, LPARAM);

PROC pfnOrigDefWindowProcA = GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");

extern LRESULT Hook_DefWindowProcACustom(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Hook_DefWindowProcA(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
    Hook_DefWindowProcACustom(hWnd, Msg, wParam, lParam);

	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
	LRESULT LPResult = ((PFNDEFWINDOWPROCA)pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam);

	return LPResult;
}


//---------------------------TextOutA

using PFNTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, LPCTSTR, int);

PROC pfnOrigTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");

BOOL WINAPI Hook_TextOutA(
    HDC hdc,           // �f�o�C�X�R���e�L�X�g�̃n���h��
    int nXStart,       // �J�n�ʒu�i��_�j�� x ���W
    int nYStart,       // �J�n�ʒu�i��_�j�� y ���W
    LPCTSTR lpString,  // ������
    int cbString       // ������
) {

    // ��ɃJ�X�^���̕������s�B
    BOOL nResult = ((PFNTEXTOUTA)pfnOrigTextOutA)(hdc, nXStart, nYStart, lpString, cbString);

    return nResult;
}


//---------------------------ExtTextOutA

using PFNEXTTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, UINT, const RECT *, LPCSTR, UINT, const INT *);

PROC pfnOrigExtTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "ExtTextOutA");

/*
* 1�ӏ���(2�ӏ��ڂ̂�����)
007E2F90  |. 6A 00          |PUSH 0                                  ; /pSpacing = NULL
007E2F92  |. 56             |PUSH ESI                                ; |StringSize
007E2F93  |. 51             |PUSH ECX                                ; |String
007E2F94  |. 8B0F           |MOV ECX,DWORD PTR DS:[EDI]              ; |
007E2F96  |. 6A 00          |PUSH 0                                  ; |pRect = NULL
007E2F98  |. 6A 00          |PUSH 0                                  ; |Options = 0
007E2F9A  |. 8D4410 01      |LEA EAX,DWORD PTR DS:[EAX+EDX+1]        ; |
007E2F9E  |. 8B5424 4C      |MOV EDX,DWORD PTR SS:[ESP+4C]           ; |
007E2FA2  |. 50             |PUSH EAX                                ; |Y
007E2FA3  |. 41             |INC ECX                                 ; |
007E2FA4  |. 51             |PUSH ECX                                ; |X
007E2FA5  |. 52             |PUSH EDX                                ; |hDC
007E2FA6  |. FF15 44508700  |CALL DWORD PTR DS:[<&GDI32.ExtTextOutA>>; \ExtTextOutA

*�@2�ӏ��� (1�ӏ��ڂ̂�����)
007E2FCA  |. 6A 00          |PUSH 0                                  ; /pSpacing = NULL
007E2FCC  |. 56             |PUSH ESI                                ; |StringSize
007E2FCD  |. 8B77 04        |MOV ESI,DWORD PTR DS:[EDI+4]            ; |
007E2FD0  |. 52             |PUSH EDX                                ; |String
007E2FD1  |. 6A 00          |PUSH 0                                  ; |pRect = NULL
007E2FD3  |. 6A 00          |PUSH 0                                  ; |Options = 0
007E2FD5  |. 03C6           |ADD EAX,ESI                             ; |
007E2FD7  |. 50             |PUSH EAX                                ; |Y
007E2FD8  |. 8B07           |MOV EAX,DWORD PTR DS:[EDI]              ; |
007E2FDA  |. 50             |PUSH EAX                                ; |X
007E2FDB  |. 51             |PUSH ECX                                ; |hDC
007E2FDC  |. FF15 44508700  |CALL DWORD PTR DS:[<&GDI32.ExtTextOutA>>; \ExtTextOutA


*/
BOOL WINAPI Hook_ExtTextOutA(
	HDC hdc,           // �f�o�C�X�R���e�L�X�g�̃n���h��
	int nXStart,       // �J�n�ʒu�i��_�j�� x ���W
	int nYStart,       // �J�n�ʒu�i��_�j�� y ���W
	UINT fuOptions,    // �I�v�V����
	const RECT *lprc,  // �N���b�s���O��`
	LPCSTR lpString,   // ������
	UINT cbCount,      // ������
	const INT *lpDx    // �����Ԋu�z��
) {
    // �����Œ��ڑ��삷��K�v�͂Ȃ��Ȃ����Bhook_textdraw.cpp�ő���
	// OutputDebugStream("ExtTextOutA:%s\n", lpString);
    // OutputDebugStream("ExtTextOutAdd:%x\n", lpString);
    // OutputDebugStream("�֐��̏ꏊ%x\n", Hook_ExtTextOutA);

	// ��ɃJ�X�^���̕������s�B
	BOOL nResult = ((PFNEXTTEXTOUTA)pfnOrigExtTextOutA)(hdc, nXStart, nYStart, fuOptions, lprc, lpString, cbCount, lpDx);

	return nResult;
}



//---------------------------CreateFontA
using PFNCREATEFONTA = HFONT (WINAPI *)(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR);

PROC pfnOrigCreateFontA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");

HFONT WINAPI Hook_CreateFontA(
    int    cHeight,
    int    cWidth,
    int    cEscapement,
    int    cOrientation,
    int    cWeight,
    DWORD  bItalic,
    DWORD  bUnderline,
    DWORD  bStrikeOut,
    DWORD  iCharSet,
    DWORD  iOutPrecision,
    DWORD  iClipPrecision,
    DWORD  iQuality,
    DWORD  iPitchAndFamily,
    LPCSTR pszFaceName
) {
    HFONT hFont = NULL;
    // �l�r ���� �Ȃ� ���� ������
    if (std::string(pszFaceName) == "�l�r ����") {
        std::string strOverrideFontName = getNB10FontName();
        if (std::string(strOverrideFontName) != "") {
            hFont = ((PFNCREATEFONTA)pfnOrigCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, strOverrideFontName.c_str());
			return hFont;
        }
    }

    hFont = ((PFNCREATEFONTA)pfnOrigCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
    // �t�H���g�t�@�~���[���w��̂��̂ŏ㏑������
    return hFont;
}

//---------------------------SetMenu
using PFNSETMENU = BOOL(WINAPI *)(HWND, HMENU);



//---------------------------ReleaseDC

using PFNRELEASEDC = int(WINAPI *)(HWND, HDC);

PROC pfnOrigReleaseDC = GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");

int WINAPI Hook_ReleaseDC(
    HWND hWnd,  // �E�B���h�E�̃n���h��
    HDC hDC     // �f�o�C�X�R���e�L�X�g�̃n���h��
) {

	// ���̂��̂��Ăяo��
	int nResult = ((PFNRELEASEDC)pfnOrigReleaseDC)(hWnd, hDC);

	return nResult;
}


//---------------------------CreateWindowExA

using PFNCREATEWINDOWEXA = HWND(WINAPI *)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);

PROC pfnOrigCreateWindowExA = GetProcAddress(GetModuleHandleA("user32.dll"), "CreateWindowExA");

HWND WINAPI Hook_CreateWindowExA(
    DWORD     dwExStyle,        // �g���E�B���h�E�X�^�C��
    LPCSTR    lpClassName,      // �E�B���h�E�N���X��
    LPCSTR    lpWindowName,     // �E�B���h�E��
    DWORD     dwStyle,          // �E�B���h�E�X�^�C��
    int       x,                // �E�B���h�E�̍������ x ���W
    int       y,                // �E�B���h�E�̍������ y ���W
    int       nWidth,           // �E�B���h�E�̕�
    int       nHeight,          // �E�B���h�E�̍���
    HWND      hWndParent,       // �e�E�B���h�E�̃n���h��
    HMENU     hMenu,            // ���j���[�̃n���h��
    HINSTANCE hInstance,        // �C���X�^���X�̃n���h��
    LPVOID    lpParam           // �쐬�f�[�^
) {

    OutputDebugStream("��%d", nWidth);
    OutputDebugStream("��%d", nHeight);
    OutputDebugStream("dwStyle%d", dwStyle);

    // �E�B���h�E��Ԃ��Ȃ�Ƃ��ۂ�
    // HWND hWnd = ((PFNCREATEWINDOWEXA)pfnOrigCreateWindowExA)(0, lpClassName, lpWindowName, 0, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);


    // ���̂��̂��Ăяo��
	HWND hWnd = ((PFNCREATEWINDOWEXA)pfnOrigCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return hWnd;
}

//---------------------------SetWindowLongA

using PFNSETWINDOWLONGA = LONG(WINAPI *)(HWND, int, LONG);

PROC pfnOrigSetWindowLongA = GetProcAddress(GetModuleHandleA("user32.dll"), "SetWindowLongA");

int count = 0;
LONG WINAPI Hook_SetWindowLongA(
    HWND hWnd,      // �E�B���h�E�̃n���h��
    int nIndex,     // �E�B���h�E�̃v���p�e�B
    LONG dwNewLong  // �V�����l
) {
    OutputDebugStream("SetWindowLongA�̌Ăяo��\n");
    if (nIndex == GWL_STYLE) {
        OutputDebugStream("GWL_STYLE�̌Ăяo��\n");
    }
    if (nIndex == GWL_EXSTYLE) {
        OutputDebugStream("GWL_EXSTYLE�̌Ăяo��\n");
    }
    else {
        OutputDebugStream("%x�̌Ăяo��\n", nIndex);

    }
	// ���̂��̂��Ăяo��
	LONG nResult = ((PFNSETWINDOWLONGA)pfnOrigSetWindowLongA)(hWnd, nIndex, dwNewLong);

    // LONG nResult = ((PFNSETWINDOWLONGA)pfnOrigSetWindowLongA)(hWnd, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE);

	return nResult;
}

//---------------------------CreateFileA

using PFNCREATEFILEA = HANDLE(WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

HANDLE hFileKAOCG = NULL;
HANDLE hFileKAHOU = NULL;
HANDLE hFileKAMON = NULL;


PROC pfnOrigCreateFileA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
// extern HANDLE Hook_CreateFileACustom(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE WINAPI Hook_CreateFileA(
    LPCSTR lpFileName, // �t�@�C����
    DWORD dwDesiredAccess, // �A�N�Z�X���@
    DWORD dwShareMode, // ���L���@
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // �Z�L�����e�B�L�q�q
    DWORD dwCreationDisposition, // �쐬���@
    DWORD dwFlagsAndAttributes, // �t�@�C������
    HANDLE hTemplateFile // �e���v���[�g�t�@�C���̃n���h��
) {
    // ��ɃJ�X�^���̕������s�B
    // Hook_CreateFileACustom(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    OutputDebugStream("CreateFileA:%s\n", lpFileName);
    string filename = string(lpFileName);


    HANDLE nResult;

    // JS������t�@�C�����̕ύX�v��������΁A����B
   // string jsOverrideFilePath = callJSModRequestFile(lpFileName);
    // �f�t�H���g�Ń`�F�b�N����I�[�o�[���C�h�t�@�C��
    string dfOverrideFilePath = string("OVERRIDE/") + lpFileName;
    /*
    if (jsOverrideFilePath.size() > 0) {
        OutputDebugStream("�t�@�C�������㏑�����܂��B%s\n", jsOverrideFilePath.c_str());
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(jsOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
    else*/ if (isFileExists(dfOverrideFilePath)) {
        // ���̂���
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(dfOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
    else {
        // ���̂���
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }


    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) { return std::toupper(c); });
    if (filename == "DATA/KAOCG.NBX") {
        OutputDebugStream("��CreateFileA:" + std::string(lpFileName) + "\n");
        hFileKAOCG = nResult;
    }
    else if (filename == "DATA/KAHOU.NBX") {
        OutputDebugStream("��CreateFileA:" + std::string(lpFileName) + "\n");
        hFileKAHOU = nResult;
    }
    else if (filename == "DATA/KAMON.NBX") {
        OutputDebugStream("��CreateFileA:" + std::string(lpFileName) + "\n");
        hFileKAMON = nResult;
    }



    // ���̂���
    return nResult;
}


//---------------------------SetFilePointer

using PFNSETFILEPOINTER = DWORD(WINAPI*)(HANDLE, LONG, PLONG, DWORD);

PROC pfnOrigSetFilePointer = GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetFilePointer");

const int lDistanceToMoveFirstFace = 96100;  // ���ۂɐD�c�M���ɃA�N�Z�X��������lDistanceToMove�̒l���
const int lDistanceToMoveFirstPallette = 25281992;// ���ۂɐD�c�M���ɃA�N�Z�X��������lDistanceToMove�̒l���

const int lDistanceToMoveFirstKahouPic = 1676; // ���ۂɃc�N���i�X�ɃA�N�Z�X��������lDistanceToMove�̒l���
const int lDistanceToMoveFirstKahouPallette = 549336;// ���ۂɃc�N���i�X�ɃA�N�Z�X��������lDistanceToMove�̒l���

const int lDistanceToMoveFirstKamonPic = 10080; // ���ۂɍŏ��̉Ɩ�ɃA�N�Z�X��������lDistanceToMove�̒l���
const int lDistanceToMoveFirstKamonPallette = 71384; // ���ۂɍŏ��̉Ɩ�ɃA�N�Z�X��������lDistanceToMove�̒l���


int nTargetKaoID = -1;

int nTargetKahouGazouID = -1;

int nTargetKamonID = -1;

// extern DWORD Hook_SetFilePointerCustom(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
DWORD WINAPI Hook_SetFilePointer(
    HANDLE hFile, // �t�@�C���̃n���h��
    LONG lDistanceToMove, // �ړ���
    PLONG lpDistanceToMoveHigh, // �ړ��ʂ̏�� 32 �r�b�g
    DWORD dwMoveMethod // �ړ����@
) {
    // ��ɃJ�X�^���̕������s�B
    // Hook_SetFilePointerCustom(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);

    // ���̂���
    DWORD nResult = ((PFNSETFILEPOINTER)pfnOrigSetFilePointer)(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
    if (hFileKAOCG == hFile) {
        // �p���b�g��菬�����Ȃ��
        if (lDistanceToMoveFirstFace <= lDistanceToMove && lDistanceToMove < lDistanceToMoveFirstPallette) {
            const int header_size = lDistanceToMoveFirstFace; // �D�c�M����1�Ԗڂ̉摜�܂ł��w�b�_�[�t�@�C��

            nTargetKaoID = (lDistanceToMove - header_size) / (KAO_PIC_WIDTH * KAO_PIC_HIGHT);
            OutputDebugStream("��SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
            OutputDebugStream("��ID:%d\n", nTargetKaoID);
        }
        else if (lDistanceToMoveFirstPallette >= lDistanceToMove) {
            const int header_size = lDistanceToMoveFirstPallette; // �D�c�M����1�Ԗڂ̃p���b�g�ʒu���N�_�Ƃ���...
            OutputDebugStream("��SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
        }
        else {
            nTargetKaoID = -1;
        }
    }
    else if (hFileKAHOU == hFile) {
        if (lDistanceToMoveFirstKahouPic <= lDistanceToMove && lDistanceToMove < lDistanceToMoveFirstKahouPallette) {
            const int header_size = lDistanceToMoveFirstKahouPic; // �c�N���i�X��1�Ԗڂ̉摜�܂ł��w�b�_�[�t�@�C��
            nTargetKahouGazouID = (lDistanceToMove - header_size) / (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
            OutputDebugStream("�ƕ�SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
            OutputDebugStream("�ƕ�摜ID:%d\n", nTargetKahouGazouID);
        }
        else if (lDistanceToMoveFirstKahouPallette >= lDistanceToMove) {
            const int header_size = lDistanceToMoveFirstKahouPallette; // �ƕ�摜��1�Ԗڂ̃p���b�g�ʒu���N�_�Ƃ���...
            OutputDebugStream("�ƕ�SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");

        }
        else {
            nTargetKahouGazouID = -1;
        }
    }
    else if (hFileKAMON == hFile) {
        if (lDistanceToMoveFirstKamonPic <= lDistanceToMove && lDistanceToMove < lDistanceToMoveFirstKamonPallette) {
            const int header_size = lDistanceToMoveFirstKamonPic; // �ŏ���1�Ԗڂ̉摜�܂ł��w�b�_�[�t�@�C��
            nTargetKamonID = (lDistanceToMove - header_size) / (KAMON_PIC_WIDTH * KAMON_PIC_WIDTH);
            OutputDebugStream("�Ɩ�SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
            OutputDebugStream("�Ɩ�摜ID:%d\n", nTargetKamonID);
        }
        else if (lDistanceToMoveFirstKamonPallette >= lDistanceToMove) {
            const int header_size = lDistanceToMoveFirstKahouPallette; // �Ɩ�摜��1�Ԗڂ̃p���b�g�ʒu���N�_�Ƃ���...
            OutputDebugStream("�Ɩ�SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");

        }
        else {
			nTargetKamonID = -1;
		}
    }

    return nResult;
}


//---------------------------ReadFile

using PFNREADFILE = BOOL(WINAPI*)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

PROC pfnOrigReadFile = GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReadFile");

extern BOOL Hook_ReadFileCustom_BushouKao(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_BushouKaoPallette(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_KahouGazou(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_KahouGazouPallete(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_KamonGazou(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_KamonGazouPallete(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

BOOL WINAPI Hook_ReadFile(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    // ���̂���
    BOOL nResult = ((PFNREADFILE)pfnOrigReadFile)(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

    if (hFileKAOCG == hFile) {
        if (nNumberOfBytesToRead == 1024) {
            OutputDebugStream("���ǂݍ��ރo�C�g��%d\n", nNumberOfBytesToRead);
            Hook_ReadFileCustom_BushouKaoPallette(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

        } else if(nNumberOfBytesToRead == (KAO_PIC_HIGHT*KAO_PIC_WIDTH)) {
			OutputDebugStream("���ǂݍ��ރo�C�g��%d\n", nNumberOfBytesToRead);
			Hook_ReadFileCustom_BushouKao(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
		}
    }
    else if (hFileKAHOU == hFile) {
        if (nNumberOfBytesToRead == 1024) {
            OutputDebugStream("�ƕ�ǂݍ��ރo�C�g��%d\n", nNumberOfBytesToRead);
            Hook_ReadFileCustom_KahouGazouPallete(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
        }
        else if (nNumberOfBytesToRead == (KAHOU_PIC_HIGHT * KAHOU_PIC_WIDTH)) {
            OutputDebugStream("�ƕ�ǂݍ��ރo�C�g��%d\n", nNumberOfBytesToRead);
            Hook_ReadFileCustom_KahouGazou(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
        }
    }
    else if (hFileKAMON == hFile) {
        if (nTargetKamonID == -1) { nTargetKamonID = 0; } // �ŏ��̓ǂݎ��
        if (nNumberOfBytesToRead == 1024) {
            OutputDebugStream("�Ɩ�ǂݍ��ރo�C�g��%d, %d\n", nNumberOfBytesToRead, nTargetKamonID);
            Hook_ReadFileCustom_KamonGazouPallete(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
        }
        else if (nNumberOfBytesToRead == (KAMON_PIC_HIGHT * KAMON_PIC_WIDTH)) {
            OutputDebugStream("�Ɩ�ǂݍ��ރo�C�g��%d\n", nNumberOfBytesToRead, nTargetKamonID);
            Hook_ReadFileCustom_KamonGazou(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
        }
    }
    return nResult;
}



//---------------------------IsDebuggerPresent

using PFNISDEBUGGERPRESENT = BOOL(WINAPI *)();

PROC pfnOrigIsDebuggerPresent = GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");

BOOL WINAPI Hook_IsDebuggerPresent() {

	// ���̂���
	BOOL nResult = ((PFNISDEBUGGERPRESENT)pfnOrigIsDebuggerPresent)();

	return FALSE;
}




/*----------------------------------------------------------------*
 HOOK�n����
 *----------------------------------------------------------------*/
bool isHookDefWindowProcA = false;
bool isHookTextOutA = false;
bool isHookExtTextOutA = false;
bool isHookCreateFontA = false;
bool isHookReleaseDC = false;
bool isHookCreateFileA = false;
bool isHookSetFilePointer = false;
bool isHookReadFile = false;
bool isHookCreateWindowExA = false;
bool isHookSetWindowLongA = false;
bool isHookIsDebuggerPresent = false;

void hookFunctionsReplace() {

    PROC pfnOrig;
    if (!isHookDefWindowProcA) {
        isHookDefWindowProcA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
    }
    if (!isHookTextOutA) {
        isHookTextOutA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");
        ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_TextOutA);
    }
if (!isHookExtTextOutA) {
		isHookExtTextOutA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "ExtTextOutA");
		ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_ExtTextOutA);
	}
    if (!isHookCreateFontA) {
		isHookCreateFontA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");
		ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_CreateFontA);
    }
    if (!isHookReleaseDC) {
		isHookReleaseDC = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");
		ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_ReleaseDC);
	}
    if (!isHookCreateFileA) {
        isHookCreateFileA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
        ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_CreateFileA);
    }
    if (!isHookSetFilePointer) {
		isHookSetFilePointer = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetFilePointer");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_SetFilePointer);
	}
    if (!isHookReadFile) {
		isHookReadFile = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReadFile");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_ReadFile);
	}
    if (!isHookCreateWindowExA) {
        isHookCreateWindowExA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "CreateWindowExA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_CreateWindowExA);
    }
    if (!isHookSetWindowLongA) {
		isHookSetWindowLongA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "SetWindowLongA");
		ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_SetWindowLongA);
	}
    if (!isHookIsDebuggerPresent) {
		isHookIsDebuggerPresent = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_IsDebuggerPresent);
	}
}