#include <file.hpp>
#include <sstream>
#include <locale>

using namespace spx;

void MemoryFile::load()
{
    if (check())
    {
    //     std::wifstream wif(filename);
    //     wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    //     std::wstringstream wss;
    //     wss << wif.rdbuf();

        std::stringstream ss;
        // ss.imbue(std::locale::empty(), )
        ss << file.rdbuf();
        buffer = ss.str();
    }
}