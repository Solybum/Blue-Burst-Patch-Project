#pragma once

int gcd(int a, int b);

void PatchNOP(int addr, int size);
void PatchCALL(int addrIn, int addrOut, int dest);
void PatchJMP(int addrIn, int addrOut, int dest);

void Log(const WCHAR* fmt, ...);
