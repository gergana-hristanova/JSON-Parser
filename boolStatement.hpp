#pragma once

#include "statement.hpp"

class BoolStatement : public Statement
{
public:
    BoolStatement(bool);

    Statement* copy() const override;

    void print(std::ostream&) const override;

private:
    bool value;
};