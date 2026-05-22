#include "array_statement.hpp"

ArrayStatement::ArrayStatement() : array() {}

ArrayStatement& ArrayStatement::add(Statement* s)
{
    array.push_back(s);

    return *this;
}

Statement* ArrayStatement::copy() const
{
    ArrayStatement* new_array = new ArrayStatement();

    for (Statement* s : array)
    {
        new_array->add(s->copy());
    }
    
    return new_array;
}

namespace
{
void write_indent(std::ostream& os, unsigned indent_spaces, unsigned depth)
{
    for (unsigned i = 0; i < indent_spaces * depth; ++i)
    {
        os << ' ';
    }
}
}

void ArrayStatement::print(std::ostream& os) const
{
    os << "[ ";
    for (size_t i = 0; i < array.size(); ++i)
    {
        if (i > 0)
        {
            os << ", ";
        }

        array[i]->print(os);
    }
    
    os << " ]";
}

void ArrayStatement::pretty_print(std::ostream& os, unsigned indent_spaces, unsigned current_indent) const
{
    if (array.empty())
    {
        os << "[]";
        return;
    }

    os << "[\n";
    for (std::size_t i = 0; i < array.size(); ++i)
    {
        write_indent(os, indent_spaces, current_indent + 1);
        array[i]->pretty_print(os, indent_spaces, current_indent + 1);
        if (i + 1 < array.size())
        {
            os << ',';
        }
        os << '\n';
    }
    write_indent(os, indent_spaces, current_indent);
    os << ']';
}

ArrayStatement::~ArrayStatement()
{
    for (Statement* s : array)
    {
        delete s;
    }
}
