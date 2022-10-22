#pragma once

static const wchar_t* clientName = L"psobb.exe";

// Main GameGuard call, nop 5 bytes to disable
static const int addrMainGameGuardCall = 0x0082D315;
