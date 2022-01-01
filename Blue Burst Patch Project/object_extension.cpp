#include "object_extension.h"

MainArenaAllocFunction MainArenaAlloc = reinterpret_cast<MainArenaAllocFunction>(0x005caba4);
MainArenaDeallocFunction MainArenaDealloc = reinterpret_cast<MainArenaDeallocFunction>(0x005c2f74);
