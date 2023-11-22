#pragma once

#include <string>

void callJSModCreateWindow(HWND hWnd);

std::string callJSModRequestFont();

void callJSModMmioOpenA(char* pszFileName, char *bufOverrideFileName);

void callJSModDestroyWindow();

