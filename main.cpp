#include <fstream>
#include <iostream>

#include "statement.cpp"
#include "numberStatement.cpp"
#include "stringStatement.cpp"

#include "utils.cpp"

int main()
{
    std::ifstream sourceFile("test.json");
    std::ofstream destFile("dest.json");

    StringStatement ss = StringStatement("Helo world");

    destFile << ss;

    sourceFile.close();
    destFile.close();
}