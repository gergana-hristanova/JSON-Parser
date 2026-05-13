#pragma once

#include "statement.hpp"

class StringStatement : public Statement
{
public:
    StringStatement(std::string value);

    Statement* copy() const override;

    void print(std::ostream& os) const override;

private:
    std::string value;
};