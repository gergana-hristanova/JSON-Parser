#include <iostream>
#include <string>

bool isWhitespace(char ch)
{
    return ch <= 32;
}

void readTrimmed(std::ostream& dest, std::istream& source)
{
    while (source.peek() != -1)
    {
        char curr = static_cast<char>(source.get());
        if (!isWhitespace(curr))
        {
            dest << curr;
        }
    }
}