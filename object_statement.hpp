#pragma once

#include <vector>

#include "statement.hpp"
#include "key_value_pair.hpp"

class ObjectStatement : public Statement
{
public:
    ObjectStatement();

    ObjectStatement& add(KeyValuePair);

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

private:
    std::vector<KeyValuePair> properties;
};
