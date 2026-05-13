#include <fstream>
#include <iostream>

#include "statement.cpp"
#include "numberStatement.cpp"
#include "stringStatement.cpp"
#include "boolStatement.hpp"
#include "nullStatement.hpp"
#include "arrayStatement.hpp"

#include "utils.cpp"

int main()
{
    std::ifstream sourceFile("test.json");
    std::ofstream destFile("dest.json");

    KeyValuePair kvp = KeyValuePair(StringStatement("name"), new StringStatement("Joan"));

    destFile << kvp;

    sourceFile.close();
    destFile.close();
}