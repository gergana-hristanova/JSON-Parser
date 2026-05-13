#pragma once

#include <vector>

#include "statement.hpp"
#include "keyValuePair.hpp"

class ObjectStatement : public Statement
{
public:
    ObjectStatement();

    ObjectStatement& add(KeyValuePair kvp);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

private:
    std::vector<KeyValuePair> properties;
};