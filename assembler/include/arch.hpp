#ifndef spx_arch_hpp
#define spx_arch_hpp

#include <spx.hpp>

namespace spx
{
    namespace arch
    {
        OpCode::Opt get_opcode(View key);
        Register::Opt get_register(View key);
        bool valid_immediate_width(uint64_t val);

        u_int16_t encode_source(uint16_t encoding, Operand target);
        u_int16_t encode_destination(uint16_t encoding, Operand target);
    }
}

#endif