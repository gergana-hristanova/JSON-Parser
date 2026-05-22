#pragma once

#include "statement.hpp"

class NullStatement : public Statement
{
public:
    NullStatement();

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;
};
