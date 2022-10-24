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
#define XASM(...) asm volatile (#__VA_ARGS__)
#endif

/**
 * @brief Defines the name for a variable that will be used in inline assembly.
 * Usage:
 *  int ASM_VAR(foo) = 123;
 */
#ifdef _MSC_VER
#define ASM_VAR(x) x
#else
#define ASM_VAR(x) x asm(#x)
#endif

#ifdef _MSC_VER
#define ASM_NAME(x)
#else
#define ASM_NAME(x) asm(#x)
#endif
