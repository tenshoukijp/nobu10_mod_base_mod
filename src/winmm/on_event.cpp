#include "dllextern.h"
#include "process.h"
#include "loadmod.h"
#include "output_debug_stream.h"
#include "dllextern.h"
#include "loadmod.h"
#include "loadantidebug.h"

void onInitialize() {
    loadAntiDebug();
    loadNB10Mod();

    Mod_onInitialize();
}

void onFinalize() {
    freeNB10Mod();
}