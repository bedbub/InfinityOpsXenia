#include "Utilities.h"

static void FixSlashes(char* path) {
    for (size_t i = 0; i < strlen(path); i++) {
        if (path[i] == '/')
            path[i] = '\\';
    }
}

void CreateFilename(char* Output, const char* filename) {
    sprintf(Output, "game:\\raw\\%s", filename);
    FixSlashes(Output);
}

bool FileExists(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    CloseHandle(hFile);
    return true;
}

bool ResolveRawPath(char* Output, const char* filename) {
    const char* prefixes[] = {
        "game:\\raw\\",
        "game:\\BlackOps1\\raw\\",
        "hdd:\\raw\\",
        "d:\\raw\\"
    };

    for (int i = 0; i < 4; i++) {
        sprintf(Output, "%s%s", prefixes[i], filename);
        FixSlashes(Output);
        if (FileExists(Output))
            return true;
    }

    CreateFilename(Output, filename);
    return false;
}