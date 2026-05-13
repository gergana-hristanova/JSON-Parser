#pragma once

#include <vector>

#include "statement.hpp"
#include "keyValuePair.hpp"

class ArrayStatement : public Statement
{
public:
    ArrayStatement();

    ArrayStatement& add(const KeyValuePair& kvp);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

private:
    std::vector<KeyValuePair> array;
};