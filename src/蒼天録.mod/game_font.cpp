#include <windows.h>
#include <winuser.h>
#include <stdio.h>

#include "game_window.h"
#include "process.h"
#include "javascript_mod.h"

using namespace System;
using namespace Microsoft::Win32;
using namespace System::Drawing;
using namespace System::Windows::Forms;


BOOL fontExistCheck();
bool isFontExist = false;
void onFontInitialize() {
	isFontExist = fontExistCheck();
}

//------------------------------------------------
// 上書き用のフォント名。長いフォント名も可能。将来はJavaScriptか何か外部テキストから変更できるようにしたい。
//------------------------------------------------
char bufferCustomFontName[32] = "蒼天 明朝";
char bufferDefaultFontName[32] = "";
std::string getNB10FontName() {
	std::string js_fontname = callJSModRequestFont();
	if (js_fontname != "") {
		// フォント名を上書きする
		strcpy_s(bufferCustomFontName, js_fontname.c_str());
		return bufferCustomFontName;
	}

	else if (isFontExist) {
		return bufferCustomFontName;
	}
	// そのまま返す
	return bufferDefaultFontName;
}

ref class FontGlobal {
public:
	static System::Drawing::Text::PrivateFontCollection^ pfc;
};

BOOL fontExistCheck() {

	// システムではなく、プライベートフォントとして、ユーザー専用としてインストールしたフォントを検索する
	FontGlobal::pfc = gcnew System::Drawing::Text::PrivateFontCollection();
	RegistryKey^ key = nullptr;
	// 比較対象のフォント。
	String^ targetFontName = gcnew String(bufferCustomFontName);
	try
	{
		key = Registry::CurrentUser->OpenSubKey(R"(Software\Microsoft\Windows NT\CurrentVersion\Fonts)");
		array<String^>^ valueNames = key == nullptr ? nullptr : key->GetValueNames();
		if (valueNames != nullptr)
		{
			for each (String ^ valueName in valueNames)
			{
				String^ path = key->GetValue(valueName)->ToString();
				if (valueName->Contains(targetFontName)) {
					// フォント名が指定のフォント名を含んでいる場合は、フォントを追加する
					FontGlobal::pfc->AddFontFile(path);
				}
				FontGlobal::pfc->AddFontFile(path);
			}

			// 独自フォント一覧に指定のフォントが存在するかどうかをチェックする
			for each (System::Drawing::FontFamily ^ font in FontGlobal::pfc->Families) {
				if (font->Name == targetFontName) {
					isFontExist = true;
					return TRUE;
				}
			}
		}
	}
	catch (Exception^ /*ex1*/) {

	}
	finally
	{
		if (key != nullptr) {
			key->Close();
		}
	}

	try {
		// 独自フォントではなく最初からウィンドウズに入っているフォントを指定しているかもしれない。
		System::Drawing::Text::InstalledFontCollection^ fonts = gcnew System::Drawing::Text::InstalledFontCollection();

		for each (System::Drawing::FontFamily ^ font in fonts->Families) {
			if (font->Name == targetFontName) {
				isFontExist = true;
				return TRUE;
			}
		}
	}
	catch (Exception^ /*ex2*/) {

	}
	finally {
	}

	return FALSE;
}