#pragma once

#ifdef _MSC_VER
#define __naked __declspec(naked)
#else
#define __naked __attribute__((naked))
#endif

/**
 * @brief This macro can be used to write inline assembly that is
 * compatible with both MSVC and (simple) GNU syntax. Intel assemby syntax is required.
 */
#ifdef _MSC_VER
#define XASM(...) __asm __VA_ARGS__
#else
#define XASM(...) asm (#__VA_ARGS__)
#endif
