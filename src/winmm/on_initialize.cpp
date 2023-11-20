#include "dllextern.h"
#include "process.h"
#include "loadmod.h"

void onInitialize() {

    setDllFuncAddress();

    loadNB10Mod();

    Mod_onInitialize();
}


void onMciSendCommand() {
}

