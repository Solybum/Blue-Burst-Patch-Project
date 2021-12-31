#pragma once

#include "map.h"

enum class Episode
{
    Episode1 = 0,
    Episode2 = 1,
    Episode4 = 2,
};

enum class Difficulty
{
    Normal = 0,
    Hard = 1,
    VeryHard = 2,
    Ultimate = 3
};

/// Booleans are often 4 byte values in PSOBB
typedef uint32_t bool32;

Episode GetCurrentEpisode();
bool IsSoloMode();
Map::MapType GetCurrentMap();
Difficulty GetCurrentDifficulty();

/// Get the size of a regular array as a constexpr
template<class T, size_t N>
constexpr size_t arraySize(T (&)[N]) { return N; }

#define CONCAT_(a, b) a##b
/// Concatenate two macro arguments
#define CONCAT(a, b) CONCAT_(a, b)

#pragma pack(push, 1)
/// Vector of three float components
struct Vec3
{
    float x;
    float y;
    float z;

    /// Copying a volatile
    volatile Vec3& operator=(volatile Vec3 const& other) volatile;
};
#pragma pack(pop)
