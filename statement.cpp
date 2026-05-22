#include "statement.hpp"

void Statement::print(std::ostream& os) const
{
    os << "HELLO!";
}

void Statement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}

Statement::~Statement() {}