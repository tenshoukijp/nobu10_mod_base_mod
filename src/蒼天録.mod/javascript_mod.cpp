#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"
#include "mng_文字列変換.h"

void callJSModCreateWindow(HWND hWnd) {
	IJavaScriptMod::onメインウィンドウ生成後((int)hWnd);
}

std::string callJSModRequestFont() {
	System::String^ fontname = IJavaScriptMod::onフォント要求時();
	if (System::String::IsNullOrEmpty(fontname)) { return ""; }
	return to_native_string(fontname);
}

void callJSModDestroyWindow() {
	IJavaScriptMod::onメインウィンドウ破棄前();
}

void callJSModRequestBGM(char* pszFileName, char* bufOverrideFileName) {
	System::String^ filepath = gcnew System::String(pszFileName);
	System::String^ ret = IJavaScriptMod::on音楽要求時(filepath->ToLower());
	if (!System::String::IsNullOrEmpty(ret)) {
		strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
	}
}

void callJSModRequestSound(char* pszFileName, char* bufOverrideFileName) {
	System::String^ filepath = gcnew System::String(pszFileName);
	System::String^ ret = IJavaScriptMod::on効果音要求時(filepath->ToLower());
	if (!System::String::IsNullOrEmpty(ret)) {
		strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
	}
}


std::string callJSModRequestBushouKaoID(int iKaoID) {
	System::String^ filepath = IJavaScriptMod::on顔画像要求時(iKaoID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}

std::string callJSModRequestKahouPicID(int iPicID) {
	System::String^ filepath = IJavaScriptMod::on家宝画像要求時(iPicID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}

std::string callJSModRequestKamonPicID(int iPicID) {
	System::String^ filepath = IJavaScriptMod::on家紋画像要求時(iPicID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}


std::string callJSModRequestFile(const char* pszFileName) {
	System::String^ filepath = IJavaScriptMod::onファイル要求時(gcnew System::String(pszFileName));
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}
