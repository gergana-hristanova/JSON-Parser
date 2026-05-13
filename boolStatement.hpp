#pragma once

#include "statement.hpp"

class BoolStatement : public Statement
{
public:
    BoolStatement(bool boolean);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

private:
    bool value;
};