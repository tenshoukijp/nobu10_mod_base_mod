#pragma once

#include <windows.h>

BOOL loadNB10Mod();

void freeNB10Mod();

void Mod_onInitialize();

void Mod_onMmioOpenA(char* pszFileName, char* bufOverrideFileName);


