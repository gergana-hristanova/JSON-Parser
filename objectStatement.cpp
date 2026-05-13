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
    os << "{ ";
    for(std::size_t i = 0; i < properties.size(); ++i)
    {
        if (i > 0)
        {
            os << ", ";
        }

        KeyValuePair kvp = properties[i];
        kvp.print(os);
    }

    os << " }";
}