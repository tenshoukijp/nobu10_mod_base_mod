#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"

using PFNONCREATEWINDOW = void (WINAPI*)(HWND);
using PFNONDESTROYWINDOW = void (WINAPI*)();
using PFNONREQUESTFONT = char* (WINAPI*)();
using PFNONMMIOOPENA = void (WINAPI*)(char*, char*);

PFNONCREATEWINDOW pMod_onCreateWindow = NULL;
PFNONDESTROYWINDOW pMod_onDestroyWindow = NULL;
PFNONREQUESTFONT pMod_onRequestFont = NULL;
PFNONMMIOOPENA pMod_onMmioOpenA = NULL;


// óÛïóì`Mod.dllÇÃì«Ç›çûÇ›
HMODULE hJavaScriptModDll = NULL;

void callJSModCreateWindow(HWND hWnd) {
	hJavaScriptModDll = LoadLibrary("JavaScript.mod.dll");

	// ë∂ç›ÇµÇ»Ç¢
	if (!hJavaScriptModDll) {
		MessageBoxA(NULL, "JavaScript.mod.dll", "ÉGÉâÅ[", NULL);
	}

	if (hJavaScriptModDll) {
		pMod_onCreateWindow = (PFNONCREATEWINDOW)GetProcAddress(hJavaScriptModDll, "onCreateWindow");
		if (pMod_onCreateWindow) {
			OutputDebugStream("callJSModCreateWindow");
			pMod_onCreateWindow(hWnd);
		}
		else {
			OutputDebugStream("NOT callJSModCreateWindow!!");
		}
	}
}

std::string bufRequestFont = "";
std::string callJSModRequestFont() {
	if (hJavaScriptModDll) {
		pMod_onRequestFont = (PFNONREQUESTFONT)GetProcAddress(hJavaScriptModDll, "onRequestFont");
		if (pMod_onRequestFont) {
			OutputDebugStream("callJSModRequestFont");
			bufRequestFont = pMod_onRequestFont();
			return bufRequestFont;
		}
		else {
			OutputDebugStream("NOT callJSModRequestFont!!");
		}
	}
	return bufRequestFont;
}

void callJSModDestroyWindow() {
	if (hJavaScriptModDll) {

		pMod_onDestroyWindow = (PFNONDESTROYWINDOW)GetProcAddress(hJavaScriptModDll, "onDestroyWindow");
		if (pMod_onDestroyWindow) {
			OutputDebugStream("callJSModDestoryWindow");
			pMod_onDestroyWindow();
		}
		else {
			OutputDebugStream("NOT callJSModDestoryWindow!!");
		}

		FreeLibrary(hJavaScriptModDll);
		hJavaScriptModDll = NULL;
	}
}

void callJSModMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	if (hJavaScriptModDll) {
		pMod_onMmioOpenA = (PFNONMMIOOPENA)GetProcAddress(hJavaScriptModDll, "onMmioOpenA");
		if (pMod_onMmioOpenA) {
			OutputDebugStream("callJSModMmioOpenA");
			pMod_onMmioOpenA(pszFileName, bufOverrideFileName);
		}
		else {
			OutputDebugStream("NOT callJSModMmioOpenA!!");
		}
	}
}