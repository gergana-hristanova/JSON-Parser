#pragma once

#include "statement.hpp"

class NumberStatement : public Statement
{
public:
    NumberStatement(double value);

    Statement* copy() const override;

    friend std::ostream& operator<<(std::ostream& os, const NumberStatement& ns);

private:
    double value;
};