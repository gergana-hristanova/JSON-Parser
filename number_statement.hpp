#pragma once

#include "statement.hpp"

class NumberStatement : public Statement
{
public:
    NumberStatement(double);

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

private:
    double value;
};
