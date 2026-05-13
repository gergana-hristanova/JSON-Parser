#pragma once

#include "statement.hpp"

class NullStatement : public Statement
{
public:
    NullStatement();

    Statement* copy() const override;

    friend std::ostream& operator<<(std::ostream& os, const NullStatement& ns);
};