#include <iostream>

#include "parser.hpp"
#include "token.hpp"
#include "objectStatement.hpp"

Statement* Parser::parse(std::istream& source = std::cin)
{
    Token token;
    source >> token;

    if (token.type == Token::TokenType::NUMBER ||
        token.type == Token::TokenType::STRING ||
        token.type == Token::TokenType::BOOLEAN ||
        token.type == Token::TokenType::_NULL)
    {
        return token.data.statement;
    }
    else if (token.type == Token::TokenType::LEFT_BRACE)
    {
        Statement* s = parse(source);
        Token::expect(source, Token::TokenType::RIGHT_BRACE);
    }
}