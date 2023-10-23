#include "quest.h"

namespace Quest
{
    decltype(opcodeTable) opcodeTable = reinterpret_cast<decltype(opcodeTable)>(0x009ccc00);
    decltype(SetupOpcodeOperand1) SetupOpcodeOperand1 = reinterpret_cast<decltype(SetupOpcodeOperand1)>(0x006b1040);
    decltype(SetupOpcodeOperand11) SetupOpcodeOperand11 = reinterpret_cast<decltype(SetupOpcodeOperand11)>(0x006b1058);

    void SetOpcode(uint16_t opcode, OpcodeSetupFn setupFn, OpcodeFn opcodeFn)
    {
        uint8_t firstByte = opcode & 0xff;
        uint8_t secondByte = opcode >> 8;
        uint32_t opcodeIndex = firstByte;

        if (secondByte == 0xf8) opcodeIndex += 0x100;
        else if (secondByte == 0xf9) opcodeIndex += 0x200;

        opcodeTable[opcodeIndex].setupFn = setupFn;
        opcodeTable[opcodeIndex].opcodeFn = opcodeFn;
    }
};
