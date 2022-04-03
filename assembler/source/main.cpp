#include <iostream>
#include <sstream>
#include <chrono>

#include <spx.hpp>
#include <file.hpp>
#include <parser.hpp>

using namespace spx;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: Missing source file\n";
        return 1;
    }
    
    MemoryFile file(argv[1]);

    if(!file.check())
    {
        std::cerr << "Error: Could not read source file\n";
        return 1;
    }

    Parser parser;
    parser.parse(file);

    return 0;
}