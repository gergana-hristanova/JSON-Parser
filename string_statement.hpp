#pragma once

#include <string>

#include "statement.hpp"

class StringStatement : public Statement
{
public:
    StringStatement(std::string);

    const std::string& get_value() const;

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

private:
    std::string value;
};
