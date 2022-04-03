#include <arch.hpp>

using namespace spx;

std::unordered_map<View, OpCode> Instructions = {
    // Data
    {"mov", OpCode("mov", 16, "0000000000000000", OpCode::Type::O)},

    // Logic
    {"and", OpCode("and", 16, "0000000000000000", OpCode::Type::O)},
    {"not", OpCode("not", 16, "0000000000000000", OpCode::Type::O)},
    {"xor", OpCode("xor", 16, "0000000000000000", OpCode::Type::O)},
    {"cmp", OpCode("cmp", 16, "0000000000000000", OpCode::Type::O)},
    {"or", OpCode("or", 16, "0000000000000000", OpCode::Type::O)},

    // Arithmetic
    {"add", OpCode("add", 16, "0000000000000000", OpCode::Type::O)},
    {"sub", OpCode("sub", 16, "0000000000000000", OpCode::Type::O)},

    // Branch
    {"jmp", OpCode("jmp", 16, "0000000000000000", OpCode::Type::SO)},

    // Cmd
    {"nop", OpCode("nop", 16, "0000000000000000", OpCode::Type::S)},
    {"hlt", OpCode("hlt", 16, "0000000000000000", OpCode::Type::S)}};

OpCode::Opt arch::get_opcode(View key)
{
    if (Instructions.contains(key))
    {
        return Instructions.at(key);
    }

    return {};
}