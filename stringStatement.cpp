#include <string>
#include <ostream>

#include "stringStatement.hpp"

StringStatement::StringStatement(std::string text) : value(text) {}

Statement* StringStatement::copy() const
{
    return new StringStatement(value);
}

std::ostream& operator<<(std::ostream& os, const StringStatement& ns)
{
    return os << '\"' << ns.value << '\"';
}