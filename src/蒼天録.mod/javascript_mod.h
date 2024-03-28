#pragma once

#include <string>

void callJSModCreateWindow(HWND hWnd);

std::string callJSModRequestFont();


void callJSModRequestSound(char* pszFileName, char* bufOverrideFileName);
void callJSModRequestBGM(char* pszFileName, char* bufOverrideFileName);

void callJSModDestroyWindow();

std::string callJSModRequestFile(const char* pszFileName);

std::string callJSModRequestBushouKaoID(int iKaoID);

std::string callJSModRequestKahouPicID(int iPicID);

std::string callJSModRequestKamonPicID(int iKamonID);



