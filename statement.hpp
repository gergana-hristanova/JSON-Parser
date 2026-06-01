#pragma once

#include <ostream>

class ObjectStatement;
class ArrayStatement;

class Statement
{
public:
    virtual Statement* copy() const = 0; // virtual copy constructor

    virtual ObjectStatement* as_object();

    virtual const ObjectStatement* as_object() const;

    virtual ArrayStatement* as_array();

    virtual const ArrayStatement* as_array() const;

    virtual bool is_container() const;

    // deprecated
    virtual void print(std::ostream&) const;
    
    virtual void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const;

    virtual ~Statement();
};
