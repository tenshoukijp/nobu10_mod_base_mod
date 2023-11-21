#pragma once

#include <windows.h>

extern HWND hNB10Wnd;

extern HWND* referenceOfNB10Wnd;

#define NB10_WINDOW_CLASS_NAME "KOEI_NOBU10WINDOW"

void onCreateWindow(HWND hWnd);

void onDestroyWindow();