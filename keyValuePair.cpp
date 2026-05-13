#include <utility>
#include <stdexcept>

#include "keyValuePair.hpp"

KeyValuePair::KeyValuePair(StringStatement key, Statement* val)
    : key(key), value(val->copy())
{}

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
    return os << kvp.key << ": " << kvp.value;
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