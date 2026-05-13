#pragma once

#include "statement.hpp"

class NullStatement : public Statement
{
public:
    NullStatement();

    Statement* copy() const override;

    void print(std::ostream& os) const override;
};