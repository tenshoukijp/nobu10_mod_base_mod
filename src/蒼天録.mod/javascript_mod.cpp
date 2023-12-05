#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"
#include "mng_文字列変換.h"

void callJSModCreateWindow(HWND hWnd) {
	IJavaScriptMod::onCreateWindow((int)hWnd);
}

std::string callJSModRequestFont() {
	System::String^ fontname = IJavaScriptMod::onRequestFont();
	if (fontname == nullptr) { return ""; }
	return to_native_string(fontname);
}

void callJSModDestroyWindow() {
	IJavaScriptMod::onDestroyWindow();
}

void callJSModMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	System::String^ filepath = gcnew System::String(pszFileName);
	if (filepath->ToLower()->StartsWith("bgm\\")) {
		System::String^ ret = IJavaScriptMod::onRequestBGM(filepath->ToLower());
		if (ret != nullptr) {
			OutputDebugString("JavaScript.modのonMmioOpenA\n");
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
	else {
		System::String^ ret = IJavaScriptMod::onRequestSound(filepath->ToLower());
		OutputDebugStream("JSから返ってきた音楽名" + to_native_string(ret + "\n"));
		if (ret != nullptr) {
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
}