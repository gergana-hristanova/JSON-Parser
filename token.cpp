#include <iostream>
#include <string>
#include <stdexcept>

#include "token.hpp"
#include "number_statement.hpp"
#include "string_statement.hpp"
#include "bool_statement.hpp"
#include "null_statement.hpp"

bool Token::is_whitespace(char ch)
{
    return ch <= 32;
}

Token Token::expect(std::istream& source, Token::TokenType type)
{
    Token expected;
    source >> expected;

    if (expected.type != type)
    {
        throw std::runtime_error("Different token type expected.");
    }

    return expected;
}

// for tokenizing true, false, and null
void tokenize_keyword(std::istream& source, const std::string& expected)
{
    for (char ch : expected)
    {
        if (source.get() != ch)
        {
            throw std::runtime_error("Invalid literal token.");
        }
    }
}

std::string tokenize_string(std::istream& source)
{
    source.get(); // ignore opening quote

    if (!source)
    {
        throw std::runtime_error("Invalid termination in string token.");
    }

    std::string result;
    int next = source.get();

    while (source && next != '"')
    {
        // escaping logic
        if (next == '\\')
        {
            int escaped = source.get();
            if (!source)
            {
                throw std::runtime_error("Invalid termination in string token.");
            }

            char esc = escaped;
            switch (esc)
            {
                case '"':
                    result.push_back('"');
                    break;
                case '\\':
                    result.push_back('\\');
                    break;
                case '/':
                    result.push_back('/');
                    break;
                case 'b':
                    result.push_back('\b');
                    break;
                case 'f':
                    result.push_back('\f');
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                default:
                    throw std::runtime_error("Invalid escape in string token.");
            }
        }
        else
        {
            result.push_back(next);
        }

        next = source.get();
    }

    if (next != '"')
    {
        throw std::runtime_error("Invalid end of string token.");
    }

    return result;
}

double tokenize_number(std::istream& source)
{
    double number = 0.0;
    source >> number;

    if (!source)
    {
        throw std::runtime_error("Invalid number token.");
    }

    return number;
}

// main tokenizing logic (lexer)
std::istream& operator>>(std::istream& source, Token& token)
{
    // trim source
    int next = source.peek();
    while (source && Token::is_whitespace(next))
    {
        source.get();
        next = source.peek();
    }

    if (!source)
    {
        throw std::runtime_error("Empty input.");
    }

    char current_symbol = next;

    if (current_symbol == '-' || std::isdigit(current_symbol))
    {
        double number = tokenize_number(source);

        token.type = Token::TokenType::NUMBER;
        token.data.statement = new NumberStatement(number);
    }
    else if(current_symbol == '"')
    {
        std::string str = tokenize_string(source);

        token.type = Token::TokenType::STRING;
        token.data.statement = new StringStatement(str);
    }
    else if (current_symbol == 't')
    {
        tokenize_keyword(source, "true");

        token.type = Token::TokenType::BOOLEAN;
        token.data.statement = new BoolStatement(true);
    }
    else if (current_symbol == 'f')
    {
        tokenize_keyword(source, "false");

        token.type = Token::TokenType::BOOLEAN;
        token.data.statement = new BoolStatement(false);
    }
    else if (current_symbol == 'n')
    {
        tokenize_keyword(source, "null");

        token.type = Token::TokenType::_NULL;
        token.data.statement = new NullStatement();
    }
    else
    {
        switch (current_symbol)
        {
            case '{':
                token.type = Token::TokenType::LEFT_BRACE;
                break;
            case '}':
                token.type = Token::TokenType::RIGHT_BRACE;
                break;
            case '[':
                token.type = Token::TokenType::LEFT_BRACKET;
                break;
            case ']':
                token.type = Token::TokenType::RIGHT_BRACKET;
                break;
            case ':':
                token.type = Token::TokenType::COLON;
                break;
            case ',':
                token.type = Token::TokenType::COMMA;
                break;
            default:
                throw std::runtime_error("Invalid symbol token.");
        }

        source.get();
        token.data.symbol = current_symbol;
    }

    return source;
}