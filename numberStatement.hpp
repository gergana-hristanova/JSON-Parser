#pragma once

#include "statement.hpp"

class NumberStatement : public Statement
{
public:
    NumberStatement(double);

    Statement* copy() const override;

    void print(std::ostream&) const override;

private:
    double value;
};