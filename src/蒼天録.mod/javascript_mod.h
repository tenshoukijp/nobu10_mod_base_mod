#pragma once

#include <string>

void callJSModCreateWindow(HWND hWnd);

std::string callJSModRequestFont();

void callJSModMmioOpenA(char* pszFileName, char *bufOverrideFileName);

void callJSModRequestSound(char* pszFileName, char* bufOverrideFileName);
void callJSModRequestBGM(char* pszFileName, char* bufOverrideFileName);

void callJSModDestroyWindow();

std::string callJSModRequestFile(const char* pszFileName);
