#pragma once

#define WIN32_LEAN_AND_MEAN

#include <array>
#include <cstdint>
#include <cstring>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>
#include <windows.h>

typedef uint8_t byte;

int gcd(int a, int b);

void PatchNOP(int addr, int size);
void PatchCALL(int addrIn, int addrOut, int dest);
void PatchJMP(int addrIn, int addrOut, int dest);

void Log(const WCHAR* fmt, ...);

/// Fill a function with NOPs leaving only a RET. Only works for caller-cleanup functions.
void StubOutFunction(int addrIn, int addrOut);

std::vector<std::vector<std::string>> ReadCsvFile(const std::string& path);

template<typename T>
T NextAligned(T current, T alignment)
{
    return (current + alignment - 1) / alignment * alignment;
}

template<typename T>
struct IsStdArray : std::false_type {};

template<typename T, std::size_t N>
struct IsStdArray<std::array<T, N>> : std::true_type {};

template<typename Result, typename... Fields>
Result ParseCsvLine(const std::vector<std::string>& lineParts)
{
    auto ParseInt = [](const std::string& s)
    {
        auto radix = s.size() > 1 && s[1] == 'x'
            ? 16 : 10;
        return std::strtoul(s.c_str(), nullptr, radix);
    };

    auto ParseFloat = [](const std::string& s)
    {
        return std::strtof(s.c_str(), nullptr);
    };
    
    auto ParseString = [](const std::string& s, size_t maxLen)
    {
        return s.substr(0, maxLen);
    };

    auto result = Result();
    auto resultCursor = reinterpret_cast<char*>(&result);
    size_t i = 0;

    // Helper function for recursively processing template arguments
    auto Helper = [&]<typename Current, typename... Rest>(auto& self)
    {
        // Parse next value based on template argument and write into struct
        auto& linePart = lineParts[i++];
        if constexpr (std::is_floating_point_v<Current>)
        {
            auto val = ParseFloat(linePart);
            std::memcpy(resultCursor, &val, std::min(sizeof(val), sizeof(Current)));
        }
        else if constexpr (std::is_integral_v<Current>)
        {
            auto val = ParseInt(linePart);
            std::memcpy(resultCursor, &val, std::min(sizeof(val), sizeof(Current)));
        }
        else if constexpr (IsStdArray<Current>::value && std::is_same_v<typename Current::value_type, char>)
        {
            auto val = ParseString(linePart, sizeof(Current));
            std::memcpy(resultCursor, val.c_str(), std::min(sizeof(Current), val.size()));
        }
        else
        {
            static_assert(false, "Invalid type");
        }
        
        // Move past this field. If struct has padding between fields then this will break.
        resultCursor += sizeof(Current);

        // Terminate recursion when all template arguments have been processed
        if constexpr (sizeof...(Rest) > 0)
            self.template operator()<Rest...>(self);
    };

    Helper.template operator()<Fields...>(Helper);

    return result;
}
