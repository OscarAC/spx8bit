#ifndef spx_parser_hpp
#define spx_parser_hpp

#include <spx.hpp>
#include <lexer.hpp>
#include <file.hpp>
#include <stack>
#include <arch.hpp>

namespace spx
{

    class Parser
    {
    public:
        Parser() = default;

        void parse(MemoryFile &file);
        void parse(View source);


        // void parse_label(View view);
        // void parse_opcode(OpCode opcode, Lexer& lex);


        private:            
            std::stack<View> address;

    };
}

#endif
