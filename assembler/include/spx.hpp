#ifndef spx_hpp
#define spx_hpp

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <fstream>

namespace spx
{

    using string_t = std::string;
    using view_t = std::string_view;

    template <class T>
    struct obj
    {
        using optional = std::optional<T>;
        using ptr = std::shared_ptr<T>;

        template <class... A>
        static inline ptr make_ptr(A... args)
        {
            return std::make_shared<T>(args...);
        }
    };

    struct lexer_t
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = view_t;

        struct next_t
        {
            size_t start;
            size_t end;
        };

        lexer_t(const view_t view) : v(view), n({0, 0})
        {
            next();
        }

        lexer_t(const view_t view, const next_t next) : v(view), n(next) {}
        lexer_t(const lexer_t &o) : v(o.v), n(o.n) {}

        view_t operator*();
        lexer_t operator++();
        bool operator==(const lexer_t &other) const;
        bool operator!=(const lexer_t &other) const;

        lexer_t begin();
        lexer_t end();

    private:
        void next();

        const view_t v;
        next_t n;
    };

    struct file_t : obj<file_t>
    {
        string_t path;
        std::ifstream file;

        file_t() = default;
        explicit file_t(view_t path)
            : path(path), file(path.data(), std::ifstream::in) {}

        // no copy
        file_t(const file_t &o) = delete;
        auto operator=(const file_t &o) = delete;

        file_t(file_t &&o)
        {
            std::swap(path, o.path);
            std::swap(file, o.file);
        }

        auto operator=(file_t &&o)
        {
            std::swap(path, o.path);
            std::swap(file, o.file);
        }

        bool check()
        {
            return file.is_open() && !file.fail() && !file.bad();
        }

        ~file_t()
        {
            if (file.is_open())
            {
                file.close();
            }
        }
    };

    auto parse(view_t source) -> int;

    auto open(view_t path) -> file_t::optional;

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

    inline bool is_label(view_t token)
    {
        return token.ends_with(':');
    }

}

#endif