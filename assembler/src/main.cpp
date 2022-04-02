#include <spx.hpp>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace spx;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: Missing source file\n";
        return 1;
    }

    auto file = open(argv[1]);
    if (!file.has_value())
    {
        std::cerr << "Error: Cannot open source file\n";
        return 1;
    }
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    string_t buffer;
    std::stringstream ss;
    ss << file.value().file.rdbuf();
    buffer = ss.str();
    file.value().file.close();

    const int iters = 1;
    std::chrono::duration<double, std::micro> times[10];

    for(int i = 0; i < iters; i++){
        const auto start = std::chrono::steady_clock::now();
        parse(buffer);
        // parse(file.value());    
        const auto end = std::chrono::steady_clock::now();
        // file.value().file.clear();
        // file.value().file.seekg(0);
        times[i] = std::chrono::duration<double, std::micro>(end - start);
    }
    
    double avg = 0;
    for(int i = 0; i < iters; i++)
    {
        avg += times[i].count();
    }

    std::cout << "10 iterations avg: " << (avg/iters) << " us\n";


    return 0;
}