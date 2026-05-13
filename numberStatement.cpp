#include <ostream>

#include "numberStatement.hpp"

NumberStatement::NumberStatement(double value) : value(value) {}

Statement* NumberStatement::copy() const
{
    return new NumberStatement(value);
}

void NumberStatement::print(std::ostream& os) const
{
    os << value;
}