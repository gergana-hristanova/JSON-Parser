#pragma once

#include <vector>

#include "statement.hpp"

class ArrayStatement : public Statement
{
public:
    ArrayStatement();

    ArrayStatement& add(Statement*);

    Statement* copy() const override;

    void print(std::ostream&) const override;

    ~ArrayStatement();

private:
    std::vector<Statement*> array;
};