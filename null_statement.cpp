#include <iostream>

#include "null_statement.hpp"

NullStatement::NullStatement() {}

Statement* NullStatement::copy() const
{
    return new NullStatement();
}

void NullStatement::print(std::ostream& os) const
{
    os << "null";
}

void NullStatement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}
