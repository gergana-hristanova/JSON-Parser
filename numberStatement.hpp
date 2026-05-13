#pragma once

#include "statement.hpp"

class NumberStatement : public Statement
{
public:
    NumberStatement(double value);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

private:
    double value;
};