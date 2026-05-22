#include <iostream>

#include "bool_statement.hpp"

BoolStatement::BoolStatement(bool boolean) : value(boolean) {}

Statement* BoolStatement::copy() const
{
    return new BoolStatement(value);
}

void BoolStatement::print(std::ostream& os) const
{
    os << std::boolalpha << value;
}

void BoolStatement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}
