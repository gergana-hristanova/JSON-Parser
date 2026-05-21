#pragma once

#include "statement.hpp"

class StringStatement : public Statement
{
public:
    StringStatement(std::string);

    Statement* copy() const override;

    void print(std::ostream&) const override;

private:
    std::string value;
};