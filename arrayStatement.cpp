#include "arrayStatement.hpp"

ArrayStatement::ArrayStatement() : array() {}

ArrayStatement& ArrayStatement::add(Statement* s)
{
    array.push_back(s);

    return *this;
}

Statement* ArrayStatement::copy() const
{
    ArrayStatement* newArray = new ArrayStatement();

    for (Statement* s : array)
    {
        newArray->add(s);
    }
    
    return newArray;
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

ArrayStatement::~ArrayStatement()
{
    for (Statement* s : array)
    {
        delete s;
    }
}