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
        case Identifier:
            std::cout << "Identifier: " << token.value << std::endl;
            break;

        case Integer:
            std::cout << "Integer: " << token.value << std::endl;
            break;

        case Comment:
            std::cout << "Comment: " << token.value << std::endl;
            break;

        case Operator:
            std::cout << "Operator: " << token.value << std::endl;

        default:
            break;
        }    
    }
}