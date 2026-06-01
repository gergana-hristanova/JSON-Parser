#include "object_statement.hpp"
#include <string>

ObjectStatement::ObjectStatement() : properties() {}

ObjectStatement& ObjectStatement::add(KeyValuePair kvp)
{
    properties.push_back(kvp);

    return *this;
}

ObjectStatement* ObjectStatement::as_object()
{
    return this;
}

const ObjectStatement* ObjectStatement::as_object() const
{
    return this;
}

bool ObjectStatement::is_container() const
{
    return true;
}

const std::vector<KeyValuePair>& ObjectStatement::get_properties() const
{
    return properties;
}

std::vector<KeyValuePair>& ObjectStatement::get_properties()
{
    return properties;
}

KeyValuePair* ObjectStatement::find(const std::string& key)
{
    for (KeyValuePair& property : properties)
    {
        if (property.get_key().get_value() == key)
        {
            return &property;
        }
    }

    return nullptr;
}

const KeyValuePair* ObjectStatement::find(const std::string& key) const
{
    for (const KeyValuePair& property : properties)
    {
        if (property.get_key().get_value() == key)
        {
            return &property;
        }
    }

    return nullptr;
}

bool ObjectStatement::erase(const std::string& key)
{
    for (auto it = properties.begin(); it != properties.end(); ++it)
    {
        if (it->get_key().get_value() == key)
        {
            properties.erase(it);
            return true;
        }
    }

    return false;
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

void ObjectStatement::pretty_print(std::ostream& os, unsigned indent_spaces, unsigned current_indent) const
{
    if (properties.empty())
    {
        os << "{}";
        return;
    }

    os << "{\n";
    for (std::size_t i = 0; i < properties.size(); ++i)
    {
        write_indent(os, indent_spaces, current_indent + 1);
        KeyValuePair kvp = properties[i];
        kvp.pretty_print(os, indent_spaces, current_indent + 1);
        if (i + 1 < properties.size())
        {
            os << ',';
        }
        os << '\n';
    }
    write_indent(os, indent_spaces, current_indent);
    os << '}';
}
