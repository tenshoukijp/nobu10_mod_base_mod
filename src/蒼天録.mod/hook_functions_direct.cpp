extern void WriteAsmJumperOnSSRExeBGMFileName();

bool isWriteAsmJumperOnSSRExeBGMFileName = false;

void hookFunctionsDirect() {
	if (!isWriteAsmJumperOnSSRExeBGMFileName) {
		WriteAsmJumperOnSSRExeBGMFileName();
		isWriteAsmJumperOnSSRExeBGMFileName = true;
	}
}