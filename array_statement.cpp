#include "array_statement.hpp"

ArrayStatement::ArrayStatement() : array() {}

ArrayStatement& ArrayStatement::add(Statement* s)
{
    array.push_back(s);

    return *this;
}

ArrayStatement* ArrayStatement::as_array()
{
    return this;
}

const ArrayStatement* ArrayStatement::as_array() const
{
    return this;
}

bool ArrayStatement::is_container() const
{
    return true;
}

const std::vector<Statement*>& ArrayStatement::get_elements() const
{
    return array;
}

std::vector<Statement*>& ArrayStatement::get_elements()
{
    return array;
}

Statement* ArrayStatement::at(std::size_t index)
{
    return array.at(index);
}

const Statement* ArrayStatement::at(std::size_t index) const
{
    return array.at(index);
}

Statement* ArrayStatement::release(std::size_t index)
{
    Statement* released = array.at(index);
    array.erase(array.begin() + index);
    return released;
}

void ArrayStatement::set(std::size_t index, Statement* value)
{
    delete array.at(index);
    array.at(index) = value;
}

void ArrayStatement::insert(std::size_t index, Statement* value)
{
    array.insert(array.begin() + index, value);
}

void ArrayStatement::erase(std::size_t index)
{
    delete array.at(index);
    array.erase(array.begin() + index);
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
