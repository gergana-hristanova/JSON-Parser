#pragma once

#include "statement.hpp"

class BoolStatement : public Statement
{
public:
    BoolStatement(bool);

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

private:
    bool value;
};
