#include <fstream>
#include <iostream>

#include "statement.hpp"
#include "numberStatement.hpp"
#include "stringStatement.hpp"
#include "boolStatement.hpp"
#include "nullStatement.hpp"
#include "arrayStatement.hpp"
#include "keyValuePair.hpp"

int main()
{
    std::ifstream sourceFile("test.json");
    std::ofstream destFile("dest.json");

    KeyValuePair kvp = KeyValuePair(StringStatement("name"), new StringStatement("Joan"));

    destFile << kvp;

    sourceFile.close();
    destFile.close();
}