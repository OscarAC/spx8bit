#include <lexer.hpp>

using namespace spx;

Token Lexer::next()
{
    Token token = lex::find_next(view, pos);
    if (token.type == TokenType::Invalid || token.type == TokenType::Npos) [[unlikely]]
    {
        pos = View::npos;
    }
    else
    {
        pos = token.position.second + 1;
    }

    return token;
}

Token Lexer::peek()
{
    return lex::find_next(view, pos);
}

Token lex::create_token(TokenType type, View value, Position position)
{
    Token token;
    token.value = value;
    token.type = type;
    token.position = position;
    return token;
}

Token lex::create_token(TokenType type, uint64_t value, Position position)
{
    Token token;
    token.value = value;
    token.type = type;
    token.position = position;
    return token;
}

Token lex::create_operator_token(OperatorType type, View value, Position position)
{
    Token token;
    token.value = value;
    token.type = TokenType::Operator;
    token.oper = type;
    token.position = position;
    return token;
}

Token lex::find_with_validator(TokenType type, size_t pos, View view, bool (*validator)(const char))
{
    bool valid = true;
    size_t end = 0;
    for (; end < view.size(); end++)
    {
        const char rc = view[end];
        if (is_separator(rc))
        {
            break;
        }
        if (!validator(rc))
        {
            valid = false;
            break;
        }
    }

    return create_token(valid ? type : TokenType::Invalid, view.substr(0, end), {pos, end});
}

Token lex::find_next(View view, size_t pos)
{
    pos = skip_spaces(view, pos);
    const char c = view[pos];

    // Newline
    if (is_newline(c)) [[unlikely]]
    {
        return create_token(TokenType::NewLine, view.substr(pos, 1), {pos, pos});
    }

    // Directive
    if (is_directive_first_char(c) && is_identifier_first_char(view[pos + 1])) [[unlikely]]
    {
        size_t end = find_next_separator(view, pos);
        return create_token(TokenType::Directive, view.substr(pos, end - pos), {pos, end - 1});
    }

    // String Literals
    else if (is_literal(c)) [[unlikely]]
    {
        pos++;
        size_t end = find_next(view, pos, '\"');
        return create_token(TokenType::Literal, view.substr(pos, end - pos), {pos, end});
    }

    // Numbers
    else if (is_digit(c)) [[unlikely]]
    {
        bool ishex = c == '0' && is_hex_indicator(view[pos + 1]);
        bool isbin = c == '0' && is_binary_indicator(view[pos + 1]);
        int base = 10;
        size_t end;
        size_t tpos = pos;
        if (ishex)
        {
            base = 16;
            pos += 2;
        }
        else if (isbin)
        {
            base = 2;
            pos += 2;
        }

        end = pos + 1;
        for (; end < view.size(); end++)
        {
            const char rc = view[end];
            if (!(ishex ? is_hexdigit(rc) : is_digit(rc)))
            {
                break;
            }
        }

        View sv = view.substr(pos, end - pos);
        auto dv = parse_int(sv, base);
        if (dv.has_value())
        {
            return create_token(TokenType::Integer, dv.value(), {tpos, end - 1});
        }
        else
        {
            return create_token(TokenType::Invalid, sv, {tpos, end - 1});
        }
    }

    // Comment
    else if (is_comment(c)) [[unlikely]]
    {
        size_t end = pos + 1;
        for (; end < view.size(); end++)
        {
            if (is_newline(view[end]))
            {
                break;
            }
        }
        return create_token(TokenType::Comment, view.substr(pos, end - pos), {pos, end - 1});
    }

    // Identifiers
    else if (is_identifier_char(c)) [[likely]]
    {

        bool valid = true;
        size_t end = pos + 1;
        for (; end < view.size(); end++)
        {
            const char rc = view[end];
            if (is_separator(rc))
            {
                break;
            }
            if (!is_identifier_char(rc))
            {
                valid = false;
                break;
            }
        }

        return create_token(valid ? TokenType::Identifier : TokenType::Invalid, view.substr(pos, end - pos), {pos, end - 1});
    }

    // Operators
    switch (c)
    {
    case '[':
        return create_operator_token(LBracket, view.substr(pos, 1), {pos, pos});
    case ']':
        return create_operator_token(RBracket, view.substr(pos, 1), {pos, pos});
    case '+':
        return create_operator_token(Plus, view.substr(pos, 1), {pos, pos});
    case '-':
        return create_operator_token(Minus, view.substr(pos, 1), {pos, pos});
    }

    return create_token(TokenType::Npos, {}, {pos, View::npos});
}