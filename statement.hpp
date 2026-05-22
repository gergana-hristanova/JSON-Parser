#pragma once

#include <ostream>

class Statement
{
public:
    virtual Statement* copy() const = 0; // virtual copy constructor

    virtual void print(std::ostream&) const;
    
    virtual void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const;

    virtual ~Statement();
};
