#include "object_extension.h"

MainArenaAllocFunction MainArenaAlloc = reinterpret_cast<MainArenaAllocFunction>(0x005caba4);
MainArenaDeallocFunction MainArenaDealloc = reinterpret_cast<MainArenaDeallocFunction>(0x005c2f74);

decltype(BaseGameObjectConstructor) BaseGameObjectConstructor = reinterpret_cast<decltype(BaseGameObjectConstructor)>(0x008193cc);
decltype(BaseGameObjectDestructor) BaseGameObjectDestructor = reinterpret_cast<decltype(BaseGameObjectDestructor)>(0x00816ff8);
decltype(BaseGameObjectDestructorNoDealloc) BaseGameObjectDestructorNoDealloc = reinterpret_cast<decltype(BaseGameObjectDestructorNoDealloc)>(0x008171e8);
