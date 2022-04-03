#include <lexer.hpp>

using namespace spx;

Token Lexer::next()
{
    Token token = lexer::find_next(view, pos);
    if (token.type == Invalid) [[unlikely]]
    {
        pos = View::npos;
    }
    else
    {
        pos = token.column + token.value.size();
    }

    return token;
}

Token Lexer::peek()
{
    return lexer::find_next(view, pos);
}

Token lexer::create_token(TokenType type, View value, size_t col)
{
    Token token;
    token.column = col;
    token.value = value;
    token.type = type;
    return token;
}

Token lexer::create_operator_token(OperatorType type, View value, size_t col)
{
    Token token;
    token.column = col;
    token.value = value;
    token.type = Operator;
    token.oper = type;
    return token;
}

Token lexer::find_with_validator(TokenType type, size_t pos, View view, bool (*validator)(const char))
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

    return create_token(valid ? type : Invalid, view.substr(0, end), pos);
}

Token lexer::find_next(View view, size_t pos)
{
    pos = skip_spaces(view, pos);
    const char c = view[pos];

    // Numbers
    if (is_digit(c)) [[unlikely]]
    {
        bool ishex = c == '0' && is_hex_indicator(view[pos + 1]);
        if (ishex)
        {
            pos += 2;
        }        
        size_t end = pos + 1;
        for (; end < view.size(); end++)
        {            
            const char rc = view[end];
            if(!(ishex ? is_hexdigit(rc) : is_digit(rc)))
            {
                break;
            }            
        }

        return create_token(Integer, view.substr(pos, end - pos), pos);        
    }

    // Identifiers
    if (is_identifier_char(c)) [[likely]]
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
        
        return create_token(valid ? Identifier : Invalid, view.substr(pos, end - pos), pos);
    }

    // Comment
    if (is_comment(c))
    {
        size_t end = pos + 1;
        for (; end < view.size(); end++)
        {
            if (is_newline(view[end]))
            {
                break;
            }
        }
        return create_token(Comment, view.substr(pos, end - pos), pos);
    }

    // Operators
    switch (c)
    {
    case '[':
        return create_operator_token(LBracket, view.substr(pos, 1), pos);
    case ']':
        return create_operator_token(RBracket, view.substr(pos, 1), pos);
    }

    return create_token(Npos, {}, View::npos);
}