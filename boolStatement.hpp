#pragma once

#include "statement.hpp"

class BoolStatement : Statement
{
public:
    BoolStatement(bool boolean);

    Statement* copy() const override;

    friend std::ostream& operator<<(std::ostream& os, const BoolStatement& bs);


private:
    bool value;
};