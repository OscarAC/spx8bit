#ifndef spx_parser_hpp
#define spx_parser_hpp

#include <spx.hpp>
#include <lexer.hpp>
#include <file.hpp>
#include <stack>
#include <arch.hpp>
#include <variant>
#include <arch.hpp>
namespace spx
{    
    class Parser : public Object<Parser>
    {
    protected:
        enum State
        {
            Init,
            Label,
            Instr
        };

    public:
        Parser() = default;

        void parse(MemoryFile &file);
        void parse(View source);

        bool parse_label(Token token);
        bool parse_opcode(OpCode opcode, Lexer &lex);

        Operand::Opt parse_destination(Lexer &lex);
        Operand::Opt parse_source(Lexer &lex);

        Instruction::Opt compile(OpCode opcode);
        Instruction::Opt compile(OpCode opcode, Operand a);
        Instruction::Opt compile(OpCode opcode, Operand a, Operand b);



    private:
        State state = Init;
    };

    namespace parse
    {
        inline bool is_label(Token &token) noexcept
        {
            return token.as_view().ends_with(":");
        }
    }
}

#endif
