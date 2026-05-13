#pragma once

#include "statement.hpp"

class NumberState : public Statement
{
public:
    NumberState(double value);

    Statement* copy() const override;

    operator std::string() const override;

    friend std::ostream& operator<<(std::ostream& os, const NumberState& ns);
};