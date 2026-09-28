#include "Detour.h"

BYTE Detour::HookSection[0x10000];
DWORD Detour::HookCount = 0;

Detour::Detour(DWORD sourceAddress, DWORD targetAddress) {
    dwAddress = sourceAddress;
    dwDestination = targetAddress;
    CallOriginal = NULL;
    Stub = NULL;

    if (!sourceAddress || !targetAddress)
        return;

    Stub = (DWORD*)&HookSection[HookCount];
    HookCount += 0x20;

    memcpy(dwRestoreInstructions, (void*)sourceAddress, 0x10);
    memcpy(Stub, (void*)sourceAddress, 0x10);

    DWORD stubAddr = (DWORD)Stub;
    DWORD rel = (sourceAddress + 0x10) - (stubAddr + 0x10);
    Stub[4] = 0x48000000 | (rel & 0x03FFFFFC);
    CallOriginal = (void*(*)(...))Stub;

    DWORD dest = targetAddress;
    DWORD* src = (DWORD*)sourceAddress;
    src[0] = 0x3D600000 | ((dest >> 16) & 0xFFFF);
    src[1] = 0x616B0000 | (dest & 0xFFFF);
    src[2] = 0x7D6903A6;
    src[3] = 0x4E800420;
}

Detour::~Detour() {
    if (dwAddress)
        memcpy((void*)dwAddress, dwRestoreInstructions, 0x10);
}