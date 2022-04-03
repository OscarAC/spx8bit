#ifndef spx_arch_hpp
#define spx_arch_hpp

#include <spx.hpp>
#include <unordered_map>

namespace spx
{
    class Operand : public Object<Operand>
    {
    };

    class Register : public Operand
    {
    };

    class Immediate : public Operand
    {
    };

    struct OpCode : public Object<OpCode>
    {

        enum class Type : char
        {
            O,            
            S,
            SO
        };

        View key;
        char width;
        View encoding;        
        Type type;

        OpCode(View k, char w, View e, Type t) : key(k), width(w), encoding(e), type(t) {}
    };

    class Instruction : public Object<Instruction>
    {

    public:
        Instruction() = default;
        Instruction(Operand dest, Operand source) : dest(dest), source(source) {}

    private:
        Operand::Opt dest;
        Operand::Opt source;
    };

    namespace arch
    {
        OpCode::Opt get_opcode(View key);
    }
}

#endif