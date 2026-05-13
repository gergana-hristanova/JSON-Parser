#include <string>
#include <ostream>

#include "stringStatement.hpp"

StringStatement::StringStatement(std::string text) : value(text) {}

Statement* StringStatement::copy() const
{
    return new StringStatement(value);
}

void StringStatement::print(std::ostream& os) const
{
    os << '\"' << value << '\"';
}