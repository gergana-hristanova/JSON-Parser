#include <iostream>

#include "command_interpreter.hpp"

int main()
{
    CommandInterpreter interpreter;

    return interpreter.run(std::cin, std::cout, std::cerr);
}