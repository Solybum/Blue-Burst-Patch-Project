#include <cstdint>
#include <cstdio>
#include "helpers.h"

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    else
    {
        return gcd(b, a % b);
    }
}

void PatchNOP(int addrIn, int size)
{
    memset((void*)addrIn, 0x90, size);
}
void PatchCALL(int addrIn, int addrOut, int addrDest)
{
    int size = addrOut - addrIn;
    memset((void*)addrIn, 0x90, size);
    *(unsigned char*)addrIn = 0xE8;
    *(int*)(addrIn + 1) = addrDest - (addrIn + 5);
}
void PatchJMP(int addrIn, int addrOut, int addrDest)
{
    int size = addrOut - addrIn;
    memset((void*)addrIn, 0x90, size);
    *(unsigned char*)addrIn = 0xE9;
    *(int*)(addrIn + 1) = addrDest - (addrIn + 5);
}

void Log(const WCHAR* fmt, ...)
{
    va_list args;
    WCHAR text[4096];
    FILE* fp;
    SYSTEMTIME rawtime;

    _wfopen_s(&fp, L"log\\dll.log", L"a, ccs=UTF-16LE");
    if (fp != NULL)
    {
        GetLocalTime(&rawtime);
        va_start(args, fmt);
        vswprintf_s(text, sizeof(text) / sizeof(WCHAR), fmt, args);
        va_end(args);

        wcscat_s(text, sizeof(text) / sizeof(WCHAR), L"\n");

        fwprintf_s(fp,
            L"[%02u-%02u-%u, %02u:%02u:%02u] %s",
            rawtime.wMonth,
            rawtime.wDay,
            rawtime.wYear,
            rawtime.wHour,
            rawtime.wMinute,
            rawtime.wSecond,
            text);
        fclose(fp);
    }
}

void StubOutFunction(int addrIn, int addrOut)
{
    PatchNOP(addrIn, addrOut - addrIn);
    *(uint8_t*) addrIn = 0xc3; // ret
}
