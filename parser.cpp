#include <stdexcept>

#include "parser.hpp"
#include "token.hpp"
#include "array_statement.hpp"
#include "object_statement.hpp"
#include "key_value_pair.hpp"
#include "string_statement.hpp"

Statement* Parser::parse_object(std::istream& source)
{
    ObjectStatement* object = new ObjectStatement();

    Token token;
    source >> token;
    while (token.type != Token::TokenType::RIGHT_BRACE)
    {
        if (token.type != Token::TokenType::STRING)
        {
            throw std::runtime_error("Key in object key-value pairs must be string.");
        }

        StringStatement* key_statement = dynamic_cast<StringStatement*>(token.data.statement);
        if (key_statement == nullptr)
        {
            delete token.data.statement;
            throw std::runtime_error("Key in object key-value pairs must be string.");
        }

        StringStatement key = *key_statement;

        delete token.data.statement;

        Token::expect(source, Token::TokenType::COLON);
        Token value_token;

        source >> value_token;
        Statement* value = parse_from_token(source, value_token);
        object->add(KeyValuePair(key, value));

        Token separator;
        source >> separator;
        
        if (separator.type == Token::TokenType::RIGHT_BRACE)
        {
            token = separator;
        }
        else if (separator.type == Token::TokenType::COMMA)
        {
            source >> token;
        }
        else
        {
            throw std::runtime_error("Expected ',' or '}' delimeter in object token.");
        }
    }
    return object;
}

Statement* Parser::parse_array(std::istream& source)
{
    ArrayStatement* array = new ArrayStatement();

    Token token;
    source >> token;

    while (token.type != Token::TokenType::RIGHT_BRACKET)
        {
        Statement* value = parse_from_token(source, token);
        array->add(value);

        Token separator;
        source >> separator;
        if (separator.type == Token::TokenType::RIGHT_BRACKET)
        {
            token = separator;
        }
        else if (separator.type == Token::TokenType::COMMA)
        {
            source >> token;
        }
        else
        {
            throw std::runtime_error("Expected ',' or ']' delimeter in array token.");
        }
    }

    return array;
}

Statement* Parser::parse_from_token(std::istream& source, const Token& token)
{
    switch (token.type)
    {
        case Token::TokenType::NUMBER:
        case Token::TokenType::STRING:
        case Token::TokenType::BOOLEAN:
        case Token::TokenType::_NULL:
            return token.data.statement;
        case Token::TokenType::LEFT_BRACE:
            return parse_object(source);
        case Token::TokenType::LEFT_BRACKET:
            return parse_array(source);
        default:
            throw std::runtime_error("Unexpected token.");
    }
}

Statement* Parser::parse(std::istream& source)
{
    Token token;
    source >> token;
    return parse_from_token(source, token);
}