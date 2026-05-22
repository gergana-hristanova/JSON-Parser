#pragma once

#include <iostream>

#include "statement.hpp"

class Parser
{
private:
    static Statement* parse_from_token(std::istream& source, const Token& token);

    static Statement* parse_object(std::istream& source);

    static Statement* parse_array(std::istream& source);

    static Statement* parse_from_token(std::istream& source, const Token& token);

public:
    static Statement* parse(std::istream&);
};