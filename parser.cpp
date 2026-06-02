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
    Token separator;

    auto cleanup_token_statement = [](const Token& t){
        switch (t.type)
        {
            case Token::TokenType::NUMBER:
            case Token::TokenType::STRING:
            case Token::TokenType::BOOLEAN:
            case Token::TokenType::_NULL:
                delete t.data.statement;
                break;
            default:
                break;
        }
    };

    try
    {
        source >> token;

        while (token.type != Token::TokenType::RIGHT_BRACE)
        {
            if (token.type != Token::TokenType::STRING)
            {
                cleanup_token_statement(token);
                throw std::runtime_error("Key in object key-value pairs must be string.");
            }

            StringStatement* key_statement = dynamic_cast<StringStatement*>(token.data.statement);
            if (key_statement == nullptr)
            {
                cleanup_token_statement(token);
                throw std::runtime_error("Key in object key-value pairs must be string.");
            }

            StringStatement key = *key_statement;
            // we've copied the key; free the temporary StringStatement allocated by the tokenizer
            cleanup_token_statement(token);

            Token::expect(source, Token::TokenType::COLON);

            Token value_token;
            source >> value_token;
            Statement* value = parse_from_token(source, value_token);
            object->add(KeyValuePair(key, value));

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
                cleanup_token_statement(separator);
                throw std::runtime_error("Expected ',' or '}' delimeter in object token.");
            }
        }
    }
    catch (...)
    {
        // clean up any statements still owned by last-read tokens
        cleanup_token_statement(token);
        cleanup_token_statement(separator);
        delete object;
        throw;
    }

    return object;
}

Statement* Parser::parse_array(std::istream& source)
{
    ArrayStatement* array = new ArrayStatement();

    Token token;
    Token separator;

    auto cleanup_token_statement = [](const Token& t){
        switch (t.type)
        {
            case Token::TokenType::NUMBER:
            case Token::TokenType::STRING:
            case Token::TokenType::BOOLEAN:
            case Token::TokenType::_NULL:
                delete t.data.statement;
                break;
            default:
                break;
        }
    };

    try
    {
        source >> token;

        while (token.type != Token::TokenType::RIGHT_BRACKET)
        {
            Statement* value = parse_from_token(source, token);
            array->add(value);

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
                cleanup_token_statement(separator);
                throw std::runtime_error("Expected ',' or ']' delimeter in array token.");
            }
        }
    }
    catch (...)
    {
        cleanup_token_statement(token);
        cleanup_token_statement(separator);
        delete array;
        throw;
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

    try
    {
        return parse_from_token(source, token);
    }
    catch (...)
    {
        // if parse_from_token throws, and the token held a Statement, free it
        switch (token.type)
        {
            case Token::TokenType::NUMBER:
            case Token::TokenType::STRING:
            case Token::TokenType::BOOLEAN:
            case Token::TokenType::_NULL:
                delete token.data.statement;
                break;
            default:
                break;
        }

        throw;
    }
}