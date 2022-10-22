#pragma once

#include <Windows.h>
static const WCHAR* clientName = L"psobb.exe";

// Main GameGuard call, nop 5 bytes to disable
static const int addrMainGameGuardCall = 0x0082D315;
