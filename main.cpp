#include <iostream>

#include "command_interpreter.hpp"

int main(int argc, char* argv[])
{
    CommandInterpreter interpreter;

    if (argc > 1)
    {
        interpreter.open_file(argv[1], std::cerr);
    }

    return interpreter.run(std::cin, std::cout, std::cerr);
}