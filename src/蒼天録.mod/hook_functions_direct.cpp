extern void WriteAsmJumperOnSSRExeSoundFileName();
extern void WriteAsmJumperOnSSRExeBGMFileName();
extern void WriteAsmJumperOnSSRExeMovieFileName();
extern void WriteAsmJumperOnSSRExeTextDraw();
extern void WriteAsmJumperOnSSRExeTextDraw01();
extern void WriteAsmJumperOnSSRExeTextDraw02();
extern void WriteAsmJumperOnSSRExeTextDraw03();
extern void WriteAsmJumperOnSSRExeTextDraw04();
extern void WriteAsmJumperOnSSRExeTextDraw05();
extern void WriteAsmJumperOnSSRExeTextDraw06();
extern void WriteAsmJumperOnSSRExeTextDraw07();
extern void WriteAsmJumperOnSSRExeTextDraw08();
extern void WriteAsmJumperOnSSRExeTextDraw09();
extern void WriteAsmJumperOnSSRExeTextDraw10();
extern void WriteAsmJumperOnSSRExeTextDraw11();
extern void WriteAsmJumperOnSSRExeTextDraw12();
extern void WriteAsmJumperOnSSRExeTextDraw13();
extern void WriteAsmJumperOnSSRExeTextDraw14();
extern void WriteAsmJumperOnSSRExeTextDraw15();
extern void WriteAsmJumperOnSSRExeTextDraw16();
extern void WriteAsmJumperOnSSRExeTextDraw13Wrap();
extern void WriteAsmJumperOnSSRExeTextDraw13Prev();


bool isWriteAsmJumperOnSSRExeSoundFileName = false;
bool isWriteAsmJumperOnSSRExeBGMFileName = false;
bool isWriteAsmJumperOnSSRExeMovieFileName = false;
bool isWriteAsmJumperOnSSRExeTextDraw = false;
bool isWriteAsmJumperOnSSRExeTextDraw01 = false;
bool isWriteAsmJumperOnSSRExeTextDraw02 = false;
bool isWriteAsmJumperOnSSRExeTextDraw03 = false;
bool isWriteAsmJumperOnSSRExeTextDraw04 = false;
bool isWriteAsmJumperOnSSRExeTextDraw05 = false;
bool isWriteAsmJumperOnSSRExeTextDraw06 = false;
bool isWriteAsmJumperOnSSRExeTextDraw07 = false;
bool isWriteAsmJumperOnSSRExeTextDraw08 = false;
bool isWriteAsmJumperOnSSRExeTextDraw09 = false;
bool isWriteAsmJumperOnSSRExeTextDraw10 = false;
bool isWriteAsmJumperOnSSRExeTextDraw11 = false;
bool isWriteAsmJumperOnSSRExeTextDraw12 = false;
bool isWriteAsmJumperOnSSRExeTextDraw13 = false;
bool isWriteAsmJumperOnSSRExeTextDraw14 = false;
bool isWriteAsmJumperOnSSRExeTextDraw15 = false;
bool isWriteAsmJumperOnSSRExeTextDraw16 = false;
bool isWriteAsmJumperOnSSRExeTextDraw13Wrap = false;
bool isWriteAsmJumperOnSSRExeTextDraw13Prev = false;



void hookFunctionsDirect() {
	if (!isWriteAsmJumperOnSSRExeSoundFileName) {
		WriteAsmJumperOnSSRExeSoundFileName();
		isWriteAsmJumperOnSSRExeSoundFileName = true;
	}
	if (!isWriteAsmJumperOnSSRExeBGMFileName) {
		WriteAsmJumperOnSSRExeBGMFileName();
		isWriteAsmJumperOnSSRExeBGMFileName = true;
	}
	if (!isWriteAsmJumperOnSSRExeMovieFileName) {
		WriteAsmJumperOnSSRExeMovieFileName();
		isWriteAsmJumperOnSSRExeMovieFileName = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw) {
		WriteAsmJumperOnSSRExeTextDraw();
		isWriteAsmJumperOnSSRExeTextDraw = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw01) {
		WriteAsmJumperOnSSRExeTextDraw01();
		isWriteAsmJumperOnSSRExeTextDraw01 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw02) {
		WriteAsmJumperOnSSRExeTextDraw02();
		isWriteAsmJumperOnSSRExeTextDraw02 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw03) {
		WriteAsmJumperOnSSRExeTextDraw03();
		isWriteAsmJumperOnSSRExeTextDraw03 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw04) {
		WriteAsmJumperOnSSRExeTextDraw04();
		isWriteAsmJumperOnSSRExeTextDraw04 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw05) {
		WriteAsmJumperOnSSRExeTextDraw05();
		isWriteAsmJumperOnSSRExeTextDraw05 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw06) {
		WriteAsmJumperOnSSRExeTextDraw06();
		isWriteAsmJumperOnSSRExeTextDraw06 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw07) {
		WriteAsmJumperOnSSRExeTextDraw07();
		isWriteAsmJumperOnSSRExeTextDraw07 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw08) {
		WriteAsmJumperOnSSRExeTextDraw08();
		isWriteAsmJumperOnSSRExeTextDraw08 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw09) {
		WriteAsmJumperOnSSRExeTextDraw09();
		isWriteAsmJumperOnSSRExeTextDraw09 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw10) {
		WriteAsmJumperOnSSRExeTextDraw10();
		isWriteAsmJumperOnSSRExeTextDraw10 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw11) {
		WriteAsmJumperOnSSRExeTextDraw11();
		isWriteAsmJumperOnSSRExeTextDraw11 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw12) {
		WriteAsmJumperOnSSRExeTextDraw12();
		isWriteAsmJumperOnSSRExeTextDraw12 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw13) {
		WriteAsmJumperOnSSRExeTextDraw13();
		isWriteAsmJumperOnSSRExeTextDraw13 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw14) {
		WriteAsmJumperOnSSRExeTextDraw14();
		isWriteAsmJumperOnSSRExeTextDraw14 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw15) {
		WriteAsmJumperOnSSRExeTextDraw15();
		isWriteAsmJumperOnSSRExeTextDraw15 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw16) {
		WriteAsmJumperOnSSRExeTextDraw16();
		isWriteAsmJumperOnSSRExeTextDraw16 = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw13Wrap) {
		WriteAsmJumperOnSSRExeTextDraw13Wrap();
		isWriteAsmJumperOnSSRExeTextDraw13Wrap = true;
	}
	if (!isWriteAsmJumperOnSSRExeTextDraw13Prev) {
		WriteAsmJumperOnSSRExeTextDraw13Prev();
		isWriteAsmJumperOnSSRExeTextDraw13Prev = true;
	}
}