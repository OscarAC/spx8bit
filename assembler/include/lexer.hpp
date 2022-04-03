#ifndef spx_lexer_hpp
#define spx_lexer_hpp

#include <spx.hpp>
#include <vector>

namespace spx
{
    enum TokenType
    {
        Invalid,
        Identifier,
        Address,
        Integer,
        Comment,
        Npos,
        Operator
    };

    enum OperatorType
    {
        None,
        LBracket,
        RBracket
    };

    struct Token : Object<Token>
    {
        using List = std::vector<Token>;

        View value;
        TokenType type;
        OperatorType oper;
        size_t line;
        size_t column;
    };

    class Lexer : public Object<Lexer>
    {

    public:
        Lexer(View view) : view(view), pos(0) {}

        Token next();
        Token peek();

        inline operator bool() const
        {
            return pos != view.size() && pos != View::npos;
        }

        inline const Token operator++()
        {
            return next();
        }

    private:
        View view;
        size_t pos;
    };

    namespace lexer
    {
        Token find_next(View view, size_t pos);
        Token create_token(TokenType type, View value, size_t pos);
        Token create_operator_token(OperatorType type, View value, size_t pos);
        Token find_with_validator(TokenType type, size_t pos, View view, bool (*validator)(const char));

        inline bool is_space(const char c)
        {
            return isspace(c);
        }

        inline bool is_comment(const char c)
        {
            return c == ';';
        }

        inline bool is_newline(const char c)
        {
            return c == '\n';
        }

        inline bool is_digit(const char c)
        {
            return isdigit(c);
        }

        inline bool is_hexdigit(const char c)
        {
            return isxdigit(c);
        }

        inline bool is_hex_indicator(const char c)
        {
            return c == 'x' || c == 'X';
        }

        inline bool is_identifier_first_char(const char c)
        {
            return isalpha(c);
        }

        inline bool is_identifier_char(const char c)
        {
            return isalnum(c) || c == ':';
        }

        inline bool is_separator(const char c)
        {
            return c == ',' || is_space(c);
        }

        inline size_t skip_spaces(View view, size_t start_pos)
        {
            for (size_t i = start_pos; i < view.size(); i++)
            {
                if (!is_space(view[i]))
                {
                    return i;
                }
            }
            return View::npos;
        }
    }
}

#endif