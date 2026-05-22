#include <fstream>
#include <iostream>
#include <stdexcept>

#include "parser.hpp"

int main()
{
    try
    {
        std::ifstream source("example.json");
        if (!source)
        {
            throw std::runtime_error("Failed to open file.");
        }

        Statement* json = Parser::parse(source);
        json->pretty_print(std::cout);
        std::cout << '\n';
        delete json;

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "ERROR! " << error.what() << '\n';
        return 1;
    }
}