#include <windows.h>
#include <string>

bool isFileExists(const std::string& filename) {
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}
