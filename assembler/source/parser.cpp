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
            std::cout << "Identifier: " << token.viewValue() << std::endl;
            break;

        case Integer:
            std::cout << "Integer: " << token.intValue() << std::endl;
            break;

        case Comment:
            std::cout << "Comment: " << token.viewValue() << std::endl;
            break;

        case NewLine:
            std::cout << "NewLine" << std::endl;
            break;

        case Operator:
            std::cout << "Operator: " << token.viewValue() << std::endl;
            break;

        case Directive:
            std::cout << "Directive: " << token.viewValue() << std::endl;
            break;

        case Literal:
            std::cout << "Literal: " << token.viewValue() << std::endl;
            break;

        case Invalid:
            std::cout << "Invalid: " << token.viewValue() << std::endl;

        default:
            break;
        }
    }
}