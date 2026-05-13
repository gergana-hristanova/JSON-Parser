#include <ostream>

#include "numberStatement.hpp"

NumberStatement::NumberStatement(double value) : value(value) {}

Statement* NumberStatement::copy() const
{
    return new NumberStatement(value);
}

std::ostream& operator<<(std::ostream& os, const NumberStatement& ns)
{
    return os << ns.value;
}