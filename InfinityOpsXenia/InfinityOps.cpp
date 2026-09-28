#include "InfinityOps.h"

#define ADDR_SCR_LOAD_RAW_MP   0x8245D5A0
#define ADDR_SCR_LOAD_RAW_FF   0x8245D4C0
#define ADDR_CHECKSUM_SLOT_MP  0x83E890F8
#define ADDR_CHECKSUM_SLOT_SP  0x839AEEF8

Detour* Scr_LoadRawFile_FastFile = nullptr;
static char gScriptBuf[0x20000];

typedef char* (*LoadRawFFFn)(int scriptInstance, const char* filename);

static int LooksLikePath(const char* s) {
    if (!s) return 0;
    if ((DWORD)s < 0x10000) return 0;
    if (s[0] < 32 || s[0] > 126) return 0;
    return 1;
}

static void ClearScriptChecksum(int scriptInstance_t) {
    DWORD addr = IsMultiplayer() ? ADDR_CHECKSUM_SLOT_MP : ADDR_CHECKSUM_SLOT_SP;
    *(int*)(addr + (scriptInstance_t * 0x10)) = 0;
}

static char* CopyScript(const char* src, int len) {
    if (!src || len <= 0)
        return NULL;

    char* dst = (char*)Hunk_AllocateTempMemoryHigh(len + 1);
    if (!dst)
        return NULL;

    memcpy(dst, src, len);
    dst[len] = 0;
    return dst;
}

void* Scr_LoadRawFile_FastFileHook(void* a1, void* a2, void* a3, void* a4, void* a5) {
    const char* name = LooksLikePath((char*)a3) ? (char*)a3 : "";

    if (name[0]) {
        char RawFileBuffer[260] = { 0 };
        CreateFilename(RawFileBuffer, name);

        HANDLE hFile = CreateFile(RawFileBuffer, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD FileLength = GetFileSize(hFile, NULL);
            DWORD BytesRead = 0;
            if (FileLength > 0 && FileLength < sizeof(gScriptBuf)) {
                memset(gScriptBuf, 0, sizeof(gScriptBuf));
                ReadFile(hFile, gScriptBuf, FileLength, &BytesRead, NULL);
            }
            CloseHandle(hFile);

            if (BytesRead > 0) {
                char* scriptBuffer = CopyScript(gScriptBuf, (int)BytesRead);
                if (scriptBuffer) {
                    ClearScriptChecksum((int)a1);
                    return scriptBuffer;
                }
            }
        }
    }

    LoadRawFFFn loadFF = (LoadRawFFFn)ADDR_SCR_LOAD_RAW_FF;
    return loadFF((int)a1, name[0] ? name : (const char*)a3);
}

static DWORD WINAPI HookThread(LPVOID) {
    Sleep(5000);
    Scr_LoadRawFile_FastFile = new Detour(ADDR_SCR_LOAD_RAW_MP, (DWORD)Scr_LoadRawFile_FastFileHook);
    return 0;
}

BOOL __stdcall DllMain(HANDLE hHandle, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, HookThread, 0, 0, 0);
    if (dwReason == DLL_PROCESS_DETACH) {
        if (Scr_LoadRawFile_FastFile) delete Scr_LoadRawFile_FastFile;
    }
    return TRUE;
}