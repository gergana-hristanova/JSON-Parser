#include <iostream>

#include "nullStatement.hpp"

NullStatement::NullStatement() {}

Statement* NullStatement::copy() const
{
    return new NullStatement();
}

std::ostream& operator<<(std::ostream& os, const NullStatement& ns)
{
    return os << "null";
}