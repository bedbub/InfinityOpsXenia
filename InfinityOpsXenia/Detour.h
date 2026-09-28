#pragma once
#include <stdio.h>
#include <xtl.h>
#include <string.h>

class Detour {
private:
    DWORD dwAddress;
    DWORD dwDestination;
    DWORD dwRestoreInstructions[4];
    DWORD* Stub;

    static BYTE HookSection[0x10000];
    static DWORD HookCount;

public:
    void* (*CallOriginal)(...);

    Detour(DWORD sourceAddress, DWORD targetAddress);
    ~Detour();
};