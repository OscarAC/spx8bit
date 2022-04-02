#include <spx.hpp>
#include <iostream>

using namespace spx;

void lexer_t::next()
{
    if (n.start == v.size())
    {
        n.start = view_t::npos;
        return;
    }

    bool ct = false;
    int ni = -1;
    for (size_t i = n.start; i < v.size(); i++)
    {
        const char c = v[i];
        bool iss = is_space(c);

        if (ct && is_newline(c))
        {
            ct = false;
        }

        if (is_comment(c))
        {
            ct = true;
        }

        else
        {
            if ((iss || ct) && ni == -1)
            {
                continue;
            }
            else if (!iss && ni == -1)
            {
                ni = i;
            }
            else if (iss && ni != -1)
            {
                n.start = ni;
                n.end = i;
                return;
            }
        }
    }

    n.start = ni == -1 ? view_t::npos : ni;
    n.end = v.size();
}

lexer_t lexer_t::begin()
{
    return *this;
}

lexer_t lexer_t::end()
{
    return {v, {view_t::npos, view_t::npos}};
}

view_t lexer_t::operator*()
{
    auto t = v.substr(n.start, n.end - n.start);
    n.start = n.end;
    return t;
}

lexer_t lexer_t::operator++()
{
    next();
    return *this;
}

bool lexer_t::operator==(const lexer_t &other) const
{
    return (v.data() == other.v.data() && other.n.start == n.start);
}

bool lexer_t::operator!=(const lexer_t &other) const
{
    return !(other == *this);
}

auto spx::open(view_t path) -> file_t::optional
{
    file_t result{path};

    if (result.file.is_open())
    {
        return result;
    }

    return {};
}
