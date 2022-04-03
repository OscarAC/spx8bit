#include <arch.hpp>
#include <unordered_map>

using namespace spx;

/*
 Data Transfer Instructions
 Arithmetic and Logic Instructions

 Control Transfer (Branch) Instructions
  - Direct
  - Relative, Unconditional
  - Relative, Conditional
  - Indirect

 Control Instructions.
*/

std::unordered_map<View, OpCode> Instructions = {
    {"nop", {"nop", 16, 0b0000000000000000, "e s d", OpCode::Type::S}},
    {"jmp", {"jmp", 16, 0b1000000000000000, "e s d", OpCode::Type::SO}},
    {"hlt", {"hlt", 16, 0b1000000000000000, "e s d", OpCode::Type::S}},

    {"mov", {"mov", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"and", {"and", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"not", {"not", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"xor", {"xor", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"cmp", {"cmp", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"orr", {"orr", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"add", {"add", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
    {"sub", {"sub", 16, 0b0000000000000000, "e s d", OpCode::Type::O}},
};            

std::unordered_map<View, Register> Registers = {
    {"r0", {"r0", "00"}},
    {"r1", {"r1", "01"}},
    {"r2", {"r2", "10"}},
    {"r3", {"r3", "11"}},
};

OpCode::Opt arch::get_opcode(View key)
{
    if (Instructions.contains(key))
    {
        return Instructions.at(key);
    }

    return {};
}

Register::Opt arch::get_register(View key)
{
    if (Registers.contains(key))
    {
        return Registers.at(key);
    }

    return {};
}

bool arch::valid_immediate_width(uint64_t val)
{
    return (val ^ 0b11111111) == 0;
}

u_int16_t arch::encode_source(uint16_t encoding, Operand target)
{
    return encoding;
}

u_int16_t arch::encode_destination(uint16_t encoding, Operand target)
{
    return encoding;
}