extern void WriteAsmJumperOnSSRExeSoundFileName();
extern void WriteAsmJumperOnSSRExeBGMFileName();
extern void WriteAsmJumperOnSSRExeMovieFileName();

bool isWriteAsmJumperOnSSRExeSoundFileName = false;
bool isWriteAsmJumperOnSSRExeBGMFileName = false;
bool isWriteAsmJumperOnSSRExeMovieFileName = false;


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
}