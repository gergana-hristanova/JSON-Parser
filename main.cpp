#include <fstream>
#include <iostream>

#include "numberStatement.cpp"

#include "utils.cpp"

int main()
{
    std::ifstream sourceFile("test.json");
    std::ofstream destFile("dest.json");

    NumberStatement ns1 = NumberStatement(53);

    destFile << ns1;

    sourceFile.close();
    destFile.close();
}