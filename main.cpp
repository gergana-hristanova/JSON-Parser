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
    ArrayStatement as = ArrayStatement();
    as.add(new StringStatement("hello"));
    as.add(new BoolStatement("false"));
    as.add(new NullStatement());

    as.print(std::cout);
}