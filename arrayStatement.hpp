#pragma once

#include <vector>

#include "statement.hpp"
#include "keyValuePair.hpp"

class ArrayStatement : public Statement
{
public:
    ArrayStatement();

    ArrayStatement& add(Statement* s);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

    ~ArrayStatement();

private:
    std::vector<Statement*> array;
};