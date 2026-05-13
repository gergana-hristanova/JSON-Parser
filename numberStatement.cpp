#include <ostream>
#include <string>

#include "numberStatement.hpp"

NumberStatement::NumberStatement(double value) : value(value)
{
}

Statement* NumberStatement::copy() const
{
    return new NumberStatement(value);
}

NumberStatement::operator std::string() const
{
    return std::to_string(value);
}

std::ostream& operator<<(std::ostream& os, const NumberStatement& ns)
{
    return os << ns.value;
}