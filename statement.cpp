#include "statement.hpp"

ObjectStatement* Statement::as_object()
{
    return nullptr;
}

const ObjectStatement* Statement::as_object() const
{
    return nullptr;
}

ArrayStatement* Statement::as_array()
{
    return nullptr;
}

const ArrayStatement* Statement::as_array() const
{
    return nullptr;
}

bool Statement::is_container() const
{
    return false;
}

void Statement::print(std::ostream& os) const
{
    os << "HELLO!";
}

void Statement::pretty_print(std::ostream& os, unsigned, unsigned) const
{
    print(os);
}

Statement::~Statement() {}