#include "statement.hpp"

void Statement::print(std::ostream& os) const
{
    os << "HELLO!";
}

Statement::~Statement() {}