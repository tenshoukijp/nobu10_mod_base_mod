extern void WriteAsmJumperOnSSRExeBGMFileName();
extern void WriteAsmJumperOnSSRExeMovieFileName();

bool isWriteAsmJumperOnSSRExeBGMFileName = false;
bool isWriteAsmJumperOnSSRExeMovieFileName = false;

void hookFunctionsDirect() {
	if (!isWriteAsmJumperOnSSRExeBGMFileName) {
		WriteAsmJumperOnSSRExeBGMFileName();
		isWriteAsmJumperOnSSRExeBGMFileName = true;
	}
	if (!isWriteAsmJumperOnSSRExeMovieFileName) {
		WriteAsmJumperOnSSRExeMovieFileName();
		isWriteAsmJumperOnSSRExeMovieFileName = true;
	}
}