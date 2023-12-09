extern void WriteAsmJumperOnSSRExeSoundFileName();
extern void WriteAsmJumperOnSSRExeBGMFileName();
extern void WriteAsmJumperOnSSRExeMovieFileName();
extern void WriteAsmJumperOnSSRExeTextDraw();

bool isWriteAsmJumperOnSSRExeSoundFileName = false;
bool isWriteAsmJumperOnSSRExeBGMFileName = false;
bool isWriteAsmJumperOnSSRExeMovieFileName = false;
bool isWriteAsmJumperOnSSRExeTextDraw = false;


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
}