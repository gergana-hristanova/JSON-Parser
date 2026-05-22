#include <utility>
#include <stdexcept>

#include "key_value_pair.hpp"

KeyValuePair::KeyValuePair(StringStatement key, Statement* val)
    : key(key), value(val->copy())
{
    delete val;
}

KeyValuePair::KeyValuePair(const KeyValuePair& other)
    : key(other.key), value(other.value->copy())
{}

KeyValuePair& KeyValuePair::operator=(KeyValuePair other)
{
    KeyValuePair copy(other);
    swap(copy);

    return *this;
}

std::ostream& operator<<(std::ostream& os, const KeyValuePair& kvp)
{
    kvp.key.print(os);
    os << ": ";
    kvp.value->print(os);
    return os;
}

void KeyValuePair::print(std::ostream& os)
{
    os << *this;
}

void KeyValuePair::pretty_print(std::ostream& os, unsigned indent_spaces, unsigned current_indent)
{
    key.pretty_print(os, indent_spaces, current_indent);
    os << ": ";
    value->pretty_print(os, indent_spaces, current_indent);
}

KeyValuePair::~KeyValuePair()
{
    delete value;
}

void KeyValuePair::swap(KeyValuePair& other)
{
    std::swap(key, other.key);
    std::swap(value, other.value);
}
