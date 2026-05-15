#include <fstream>
#include <iostream>

#include "numberStatement.hpp"
#include "stringStatement.hpp"
#include "boolStatement.hpp"
#include "nullStatement.hpp"
#include "arrayStatement.hpp"
#include "objectStatement.hpp"
#include "keyValuePair.hpp"
#include "tokenizer.hpp"

int main()
{
    KeyValuePair kvp1 = KeyValuePair(StringStatement("name:"), new NullStatement());
    KeyValuePair kvp2 = KeyValuePair(StringStatement("age:"), new NumberStatement(15));
    KeyValuePair kvp3 = KeyValuePair(StringStatement("id:"), new StringStatement("A800"));
    ObjectStatement os = ObjectStatement();
    os.add(kvp1).add(kvp2).add(kvp3);

    os.print(std::cout);
    return 0;
}