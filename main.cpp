#include <fstream>
#include <iostream>

#include "numberStatement.hpp"
#include "stringStatement.hpp"
#include "boolStatement.hpp"
#include "nullStatement.hpp"
#include "arrayStatement.hpp"
#include "objectStatement.hpp"
#include "keyValuePair.hpp"
#include "token.hpp"
#include "parser.hpp"

int main()
{
    std::ifstream source("example.json");

    Statement* JSON = Parser::parse(source);

    std::cout << JSON << "---";

    return 0;
}