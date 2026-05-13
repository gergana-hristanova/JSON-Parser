#include <iostream>

#include "boolStatement.hpp"

BoolStatement::BoolStatement(bool boolean) : value(boolean) {}

Statement* BoolStatement::copy() const
{
    return new BoolStatement(value);
}

void BoolStatement::print(std::ostream& os) const
{
    os << std::boolalpha << value;
}