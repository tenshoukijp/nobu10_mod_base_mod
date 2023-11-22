#include <windows.h>
#include <string>
#include <cctype> 
#include "JavaScript.mod.h"
#include "string_convert.h"


extern "C" __declspec(dllexport) void WINAPI onCreateWindow(HWND hWnd) {
	IJavaScriptStaticLib::onCreateWindow((int)hWnd);
}

std::string bufRequestFont = "";
extern "C" __declspec(dllexport) const char* WINAPI onRequestFont() {
	bufRequestFont.clear();
	System::String^ ret = IJavaScriptStaticLib::onRequestFont();
	if (ret == nullptr) { ret = ""; }
	System::Diagnostics::Trace::WriteLine("JS‚©‚ç•Ô‚Á‚Ä‚«‚½ƒtƒHƒ“ƒg" + ret);
	bufRequestFont = String_to_string(ret);
	return bufRequestFont.c_str();
}

extern "C" __declspec(dllexport) void WINAPI onDestroyWindow() {
	IJavaScriptStaticLib::onDestroyWindow();
}

extern "C" __declspec(dllexport) void WINAPI onMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	/*
	System::String^ filepath = string_to_String(pszFileName);
	if (filepath->ToLower()->StartsWith("bgm\\") ) {
		String^ ret = IJavaScriptStaticLib::onRequestBGM(filepath);
		if (ret != nullptr) {
			strcpy_s(bufOverrideFileName, 512, String_to_string(ret).c_str());
		}
	}
	else {
		String^ ret = IJavaScriptStaticLib::onRequestSound(filepath);
		if (ret != nullptr) {
			strcpy_s(bufOverrideFileName, 512, String_to_string(ret).c_str());
		}
	}
	*/
}