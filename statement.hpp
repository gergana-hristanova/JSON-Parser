#pragma once

#include <ostream>

class Statement
{
public:
    virtual Statement* copy() const = 0; // virtual copy constructor

    virtual void print(std::ostream&) const;

    virtual ~Statement();
};
