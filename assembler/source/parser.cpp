#include <spx.hpp>
#include <parser.hpp>
#include <iostream>
#include <arch.hpp>

using namespace spx;

void Parser::parse(MemoryFile &file)
{
    parse(file.data());
}

void Parser::parse(View source)
{
    Lexer lex = source;

    while (lex)
    {
        auto token = lex.next();

        switch (token.type)
        {
        case TokenType::Identifier:
        {

            // Labels
            if (parse::is_label(token))
            {
                if (parse_label(token))
                {
                    state = Instr;
                }
            }

            // Instructions
            OpCode::Opt opcode = arch::get_opcode(token.as_view());
            if (opcode.has_value())
            {
                if (parse_opcode(opcode.value(), lex))
                {
                    state = Init;
                }
            }
        }
        break;

        case TokenType::Integer:
        case TokenType::Comment:
        case TokenType::NewLine:
        case TokenType::Operator:
        case TokenType::Directive:
        case TokenType::Literal:
        case TokenType::Invalid:
        default:
            break;
        }
    }
}

bool Parser::parse_label(Token token)
{
    return true;
}

Instruction::Opt Parser::compile(OpCode opcode)
{
    return {};
}

Instruction::Opt Parser::compile(OpCode opcode, Operand a)
{
    return {};
}

Instruction::Opt Parser::compile(OpCode opcode, Operand a, Operand b)
{
    uint16_t encoding = opcode.encoding;
    encoding = arch::encode_destination(encoding, a);
    encoding = arch::encode_source(encoding, b);    
    std::cout << "Encoding: " << encoding << std::endl;

    return {};
}

Operand::Opt parse_identifier_operand(Token token)
{
    auto reg = arch::get_register(token.as_view());
    if (reg.has_value())
    {
        return Operand{Operand::Reg, reg.value()};
    }
    return {};
}

Operand::Opt parse_operator_operand(Token token)
{
    return {};
}

Operand::Opt Parser::parse_destination(Lexer &lex)
{

    auto a = lex.next();

    if (a.type == TokenType::Identifier) [[likely]]
    {
        return parse_identifier_operand(a);
    }
    else if (a.type == TokenType::Operator)
    {

        if (a.oper == OperatorType::LBracket)
        {
            auto v = lex.next();
            auto c = lex.next();
            if (v.type == TokenType::Integer && c.oper == OperatorType::RBracket)
            {
                return Operand{Operand::Addr, Address{v.as_int()}};
            }
        }
    }
    return {};
}

Operand::Opt Parser::parse_source(Lexer &lex)
{
    auto a = lex.next();

    if (a.type == TokenType::Identifier) [[likely]]
    {
        return parse_identifier_operand(a);
    }
    else if (a.type == TokenType::Operator)
    {

        if (a.oper == OperatorType::LBracket)
        {
            auto v = lex.next();
            auto c = lex.next();
            if (v.type == TokenType::Integer && c.oper == OperatorType::RBracket)
            {
                return Operand{Operand::Addr, Address{v.as_int()}};
            }
        }
        // else if() // Expressions...
    }
    else if(a.type == TokenType::Integer)
    {
        auto val = a.as_int();
        if(arch::valid_immediate_width(val))
        {
            return Operand{Operand::Imm, Immediate{val}};
        }
    }
    return {};
}

bool Parser::parse_opcode(OpCode opcode, Lexer &lex)
{
    switch (opcode.type)
    {
    case OpCode::O:
    {

        auto a = parse_destination(lex);
        if (!a.has_value())
        {
            return false;
        }

        auto b = parse_source(lex);
        if (!b.has_value())
        {
            return false;
        }

        auto i = compile(opcode, a.value(), b.value());
        if (!i.has_value())
        {
            return false;
        }

        // push_instruction(i);
    }
    break;
    case OpCode::S:
    case OpCode::SO:
        break;
    }

    return false;
}