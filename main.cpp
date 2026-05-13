#include <fstream>
#include <iostream>

#include "utils.cpp"

int main()
{
    std::ifstream sourceFile("example.json");
    std::ofstream destFile("dest.json");

    while (sourceFile.peek() != -1)
    {
        readTrimmed(destFile, sourceFile);
    }

    sourceFile.close();
    destFile.close();
}