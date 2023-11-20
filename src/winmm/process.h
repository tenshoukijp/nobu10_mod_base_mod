#pragma once

#include <string>

extern BOOL isTargetProcessing;

BOOL IsWow64();

std::string GetCurrentProcessFullPath();

// 現在実行中の自分のプロセスのウィンドウハンドルを得る。
HWND GetCurrentWindowHandle();

// 現在のプロセスのメインクラスは、指定のクラス名と一致するか？
HWND GetNB7WindowHandle(const char* programName);