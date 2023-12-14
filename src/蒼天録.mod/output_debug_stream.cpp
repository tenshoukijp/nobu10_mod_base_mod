#include "output_debug_stream.h"

void OutputDebugStream(std::string strOutputString) {
#ifdef SUPER_RELEASE
	return;
#endif
	OutputDebugString(strOutputString.c_str());
}

void OutputDebugStream(int number) {
#ifdef SUPER_RELEASE
	return;
#endif
	OutputDebugString(std::to_string(number).c_str());
}

#pragma unmanaged
char szBufOutputDebugStream[4096] = "";
void OutputDebugStream(const char* format, ...) {
#ifdef SUPER_RELEASE
	return;
#endif

	int length = sizeof(szBufOutputDebugStream) - 1;

	va_list arg;

	va_start(arg, format);
	_vsnprintf_s(szBufOutputDebugStream, length, _TRUNCATE, format, arg);
	va_end(arg);

	/*
	// 最後の文字が改行ではない場合
	if (szBufOutputDebugStream[strlen(szBufOutputDebugStream) - 1] != '\n') {
		// 改行を足す。
		strcat(szBufOutputDebugStream, "\n");
	}
	*/

	szBufOutputDebugStream[length - 1] = '\0'; // 番兵。最悪の場合は最後の改行をすり潰す。

	// OutputDebugStringの方に出す。
	OutputDebugString(szBufOutputDebugStream);
}
#pragma managed
