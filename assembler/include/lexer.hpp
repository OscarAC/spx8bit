#ifndef spx_lexer_hpp
#define spx_lexer_hpp

#include <spx.hpp>
#include <vector>
#include <variant>
#include <charconv>

namespace spx
{
    enum TokenType
    {
        Invalid,
        Identifier,
        Address,
        Integer,
        Comment,
        Operator,
        Directive,
        Literal,
        NewLine,
        Npos,
    };

    enum OperatorType
    {
        None,
        LBracket,
        RBracket,
        Plus,
        Minus
    };

    struct Token : Object<Token>
    {
        using Value = std::variant<uint64_t, View>;
        using List = std::vector<Token>;

        template <typename T>
        void set_value(T val)
        {
            value = val;
        }

        uint64_t intValue()
        {
            return std::get<uint64_t>(value);
        }

        View viewValue()
        {
            return std::get<View>(value);
        }

        Value value;
        TokenType type;
        OperatorType oper;
        Position position;
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
        Token create_token(TokenType type, View value, Position position);
        Token create_token(TokenType type, uint64_t value, Position position);
        Token create_operator_token(OperatorType type, View value, Position position);
        Token find_with_validator(TokenType type, size_t pos, View view, bool (*validator)(const char));

        inline constexpr bool is_space(const char c) noexcept
        {
            return c == ' ' || c == '\t';
        }

        inline constexpr bool is_comment(const char c) noexcept
        {
            return c == ';';
        }

        inline constexpr bool is_newline(const char c) noexcept
        {
            return c == '\n' ||  c == '\r';
        }

        inline constexpr bool is_digit(const char c) noexcept
        {
            return isdigit(c);
        }

        inline constexpr bool is_hexdigit(const char c) noexcept
        {
            return isxdigit(c);
        }

        inline constexpr bool is_hex_indicator(const char c) noexcept
        {
            return c == 'x' || c == 'X';
        }

        inline constexpr bool is_binary_indicator(const char c) noexcept
        {
            return c == 'b' || c == 'B';
        }

        inline constexpr bool is_identifier_first_char(const char c) noexcept
        {
            return isalpha(c);
        }

        inline constexpr bool is_identifier_char(const char c) noexcept
        {
            return isalnum(c) || c == ':';
        }

        inline constexpr bool is_separator(const char c) noexcept
        {
            return c == ',' || is_space(c) || is_newline(c);
        }

        inline bool is_directive_first_char(const char c) noexcept
        {
            return c == '.';
        }

        inline bool is_literal(const char c) noexcept
        {
            return c == '\"';
        }

        inline size_t skip_spaces(View view, size_t start_pos)
        {
            size_t i = start_pos;
            for (; i < view.size(); i++)
            {
                if (!is_space(view[i]))
                {
                    return i;
                }
            }
            return i;
        }

        inline size_t find_next_separator(View view, size_t start_pos)
        {
            size_t i = start_pos;
            for (; i < view.size(); i++)
            {
                if (is_separator(view[i]))
                {
                    break;
                }
            }

            return i;
        }

        inline size_t find_next(View view, size_t start_pos, const char t)
        {
            size_t i = start_pos;
            for (; i < view.size(); i++)
            {
                if (t == view[i])
                {
                    break;
                }
            }

            return i;
        }

        inline std::optional<uint64_t> parse_int(View view, int base)
        {            
            int result;
            auto [p, ec] = std::from_chars(view.data(), view.data() + view.size(), result, base);
            if (ec != std::errc{})
            {
                return {};
            }

            return result;
        }
    }
}

#endif