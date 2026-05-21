#include "objectStatement.hpp"

ObjectStatement::ObjectStatement() : properties() {}

ObjectStatement& ObjectStatement::add(KeyValuePair kvp)
{
    properties.push_back(kvp);

    return *this;
}

Statement* ObjectStatement::copy() const
{
    ObjectStatement* newOS = new ObjectStatement();

    for (KeyValuePair kvp : properties)
    {
        newOS->add(kvp);
    }

    return newOS;
}

void ObjectStatement::print(std::ostream& os) const
{
    os << "[ " << properties[0];
    for(std::size_t i = 1; i < properties.size(); ++i)
    {
        os << ", ";

        KeyValuePair kvp = properties[i];
        kvp.print(os);
    }

    os << " ]";
}