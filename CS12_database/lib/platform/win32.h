#include <windows.h>

int dir_exist(const char *path) {
    DWORD attrib = GetFileAttributesA(path);
    if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return 1;
    }
    return 0; 
}
