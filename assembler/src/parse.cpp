#include <spx.hpp>
#include <iostream>
#include <sstream>

using namespace spx;

enum class parse_s : char
{
    init,
    label
};

int spx::parse(view_t source)
{
    int c = 0;
    parse_s s = parse_s::init;

    for (auto t : lexer_t(source))
    {
        switch (s)
        {

            case parse_s::init:
            {
                if(is_label(t))
                {
                    s = parse_s::label;
                    c++;
                }                
            }
            break;

            case parse_s::label:
            {
                
            }
            break;
        }

        // std::cout << t << std::endl;
    }

    return c;
}