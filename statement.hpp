#pragma once

#include <string>

class Statement
{
public:
    
    virtual Statement* copy() const = 0; // virtual copy constructor

    virtual ~Statement();
};
