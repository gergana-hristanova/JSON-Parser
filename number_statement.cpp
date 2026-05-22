#include <ostream>

#include "number_statement.hpp"

NumberStatement::NumberStatement(double value) : value(value) {}

Statement* NumberStatement::copy() const
{
    return new NumberStatement(value);
}

void NumberStatement::print(std::ostream& os) const
{
    os << value;
}

void NumberStatement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}
