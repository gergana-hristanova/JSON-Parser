#include <iostream>

#include "nullStatement.hpp"

NullStatement::NullStatement() {}

Statement* NullStatement::copy() const
{
    return new NullStatement();
}

void NullStatement::print(std::ostream& os) const
{
    os << "null";
}