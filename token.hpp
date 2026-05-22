#pragma once

#include <iostream>
#include <string>

#include "statement.hpp"

struct Token
{
    enum class TokenType
    {
        // structural tokens:
        LEFT_BRACE,    //  {
        RIGHT_BRACE,   //  }
        LEFT_BRACKET,  //  [
        RIGHT_BRACKET, //  ]
        COLON,         //  :
        COMMA,         //  ,

        // value tokens:
        NUMBER,
        STRING,
        BOOLEAN,
        _NULL
    };

    TokenType type;

    union
    {
        Statement* statement;
        char symbol;
    } data;

    static bool is_whitespace(char);

    void tokenize_keyword(std::istream&, const std::string&);

    static Token expect(std::istream&, Token::TokenType);

    std::string tokenize_string(std::istream&);

    double tokenize_number(std::istream&);

    friend std::istream& operator>>(std::istream&, Token&);
};
