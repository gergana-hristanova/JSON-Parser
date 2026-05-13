#pragma once

#include <string>

class Statement
{
public:
    
    virtual Statement* copy() const = 0; // virtual copy constructor

    virtual operator std::string() const = 0;

    virtual ~Statement();
};
