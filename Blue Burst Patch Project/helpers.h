#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

typedef uint8_t byte;

int gcd(int a, int b);

void PatchNOP(int addr, int size);
void PatchCALL(int addrIn, int addrOut, int dest);
void PatchJMP(int addrIn, int addrOut, int dest);

void Log(const WCHAR* fmt, ...);

/// Fill a function with NOPs leaving only a RET. Only works for caller-cleanup functions.
void StubOutFunction(int addrIn, int addrOut);
