#pragma once

#include <iostream>

#include "statement.hpp"

class Parser
{
public:
    static Statement* parse(std::istream&);
};