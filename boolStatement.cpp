#include <iostream>

#include "boolStatement.hpp"

BoolStatement::BoolStatement(bool boolean) : value(boolean) {}

Statement* BoolStatement::copy() const
{
    return new BoolStatement(value);
}

std::ostream& operator<<(std::ostream& os, const BoolStatement& bs)
{
    return os << std::boolalpha << bs.value;
}