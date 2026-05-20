#include <iostream>

#include "token.hpp"
#include "stringStatement.hpp"
#include "numberStatement.hpp"
#include "boolStatement.hpp"
#include "nullStatement.hpp"

bool Token::isWhitespace(char ch)
{
    return ch <= 32;
}

// main tokenizing logic (lexer)
std::istream& operator>>(std::istream& source, Token& token)
{
    // trim beginning of source
    while (Token::isWhitespace(source.peek()))
    {
        source.get();
    }

    char current_symbol = source.peek();

    if(current_symbol == '"')
    {
        source.get(); //ignore first "
        
        std::string str;
        std::getline(source, str, '"');

        source.get(); //ignore second "

        token.type = Token::TokenType::STRING;
        token.data.statement = new StringStatement(str);
    }
    else if (std::isdigit(current_symbol))
    {
        double number;
        source >> number;

        token.type = Token::TokenType::NUMBER;
        token.data.statement = new NumberStatement(number);
    }
    else if (current_symbol == 't')
    {
        // skip the word true
        source.get();
        source.get();
        source.get();
        source.get();

        token.type = Token::TokenType::BOOLEAN;
        token.data.statement = new BoolStatement(true);
    }
    else if (current_symbol == 'f')
    {
        // skip the word false
        source.get();
        source.get();
        source.get();
        source.get();
        source.get();

        token.type = Token::TokenType::BOOLEAN;
        token.data.statement = new BoolStatement(false);
    }
        else if (current_symbol == 'n')
    {
        // skip the word null
        source.get();
        source.get();
        source.get();

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
                // TODO THROW
                std::cerr << "ERROR! Invalid token";
        }

        token.data.symbol = current_symbol;
    }

    return source;
}