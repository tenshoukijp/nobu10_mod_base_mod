#include <windows.h>
#include <winuser.h>
#include <stdio.h>

#include "game_window.h"
#include "process.h"
#include "javascript_mod.h"



//------------------------------------------------
// 使っていない。Hook_CreateFontACustom()で処理するようにした。
//------------------------------------------------
void onFontInitialize() {
}

//------------------------------------------------
// 上書き用のフォント名。長いフォント名も可能。将来はJavaScriptか何か外部テキストから変更できるようにしたい。
//------------------------------------------------
// char bufferCustomFontName[32] = "烈風 明朝";
std::string getNB8FontName() {
	// return bufferCustomFontName;
	return callJSModRequestFont();
}