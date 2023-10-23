#pragma once

#include <cstdint>

namespace Quest
{
    using OpcodeSetupFn = void (__cdecl*)(uint32_t opcode);
    using OpcodeFn = void*;

    #pragma pack(push, 1)
    struct OpcodeHandler
    {
        OpcodeSetupFn setupFn;
        OpcodeFn opcodeFn;
    };
    #pragma pack(pop)

    extern OpcodeHandler* opcodeTable;

    extern OpcodeSetupFn SetupOpcodeOperand1;
    extern OpcodeSetupFn SetupOpcodeOperand11;

    void SetOpcode(uint16_t opcode, OpcodeSetupFn setupFn, OpcodeFn opcodeFn);
};
