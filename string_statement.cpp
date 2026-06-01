#include <string>
#include <ostream>

#include "string_statement.hpp"

StringStatement::StringStatement(std::string text) : value(text) {}

const std::string& StringStatement::get_value() const
{
    return value;
}

Statement* StringStatement::copy() const
{
    return new StringStatement(value);
}

void StringStatement::print(std::ostream& os) const
{
    os << '"' << value << '"';
}

void StringStatement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}
