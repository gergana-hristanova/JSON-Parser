#pragma once

#include "statement.hpp"

class StringStatement : public Statement
{
public:
    StringStatement(std::string value);

    Statement* copy() const override;

    friend std::ostream& operator<<(std::ostream& os, const StringStatement& ns);

private:
    std::string value;
};