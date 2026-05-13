#include "arrayStatement.hpp"

ArrayStatement::ArrayStatement() : array() {}

ArrayStatement& ArrayStatement::add(const KeyValuePair& kvp)
{
    array.push_back(kvp);

    return *this;
}

Statement* ArrayStatement::copy() const
{
    ArrayStatement* newArray = new ArrayStatement();

    for (const KeyValuePair& kvp : array)
    {
        newArray->add(kvp);
    }
    
    return newArray;
}