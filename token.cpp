#include <iostream>
#include <string>
#include <stdexcept>

#include "token.hpp"
#include "number_statement.hpp"
#include "string_statement.hpp"
#include "bool_statement.hpp"
#include "null_statement.hpp"

#include <sstream>

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
        switch (expected.type)
        {
            case Token::TokenType::NUMBER:
            case Token::TokenType::STRING:
            case Token::TokenType::BOOLEAN:
            case Token::TokenType::_NULL:
                delete expected.data.statement;
                break;
            default:
                break;
        }

        std::ostringstream ss;
        ss << "Different token type expected at line " << expected.line << " col " << expected.column << ".";
        throw std::runtime_error(ss.str());
    }

    return expected;
}

// for tokenizing true, false, and null
void tokenize_keyword(std::istream& source, const std::string& expected)
{
    // compute position for better error messages
    auto get_pos = [&source]() {
        std::istream::pos_type cur = source.tellg();
        if (cur == (std::istream::pos_type)-1) return std::pair<int,int>(1,1);
        source.clear();
        source.seekg(0);
        int line = 1;
        int col = 1;
        char ch;
        while (source && source.tellg() < cur && source.get(ch))
        {
            if (ch == '\n') { ++line; col = 1; }
            else { ++col; }
        }
        source.clear();
        source.seekg(cur);
        return std::pair<int,int>(line, col);
    };

    for (char ch : expected)
    {
        if (source.get() != ch)
        {
            auto pos = get_pos();
            std::ostringstream ss;
            ss << "Invalid literal token at line " << pos.first << " col " << pos.second << ".";
            throw std::runtime_error(ss.str());
        }
    }
}

std::string tokenize_string(std::istream& source)
{
    // compute opening position for error messages
    std::istream::pos_type start_pos = source.tellg();
    source.get(); // ignore opening quote

    if (!source)
    {
        std::pair<int,int> pos(1,1);
        if (start_pos != (std::istream::pos_type)-1)
        {
            source.clear(); source.seekg(0);
            int line = 1, col = 1; char ch;
            while (source && source.tellg() < start_pos && source.get(ch)) { if (ch=='\n'){++line; col=1;} else ++col; }
            pos = {line, col};
            source.clear(); source.seekg(start_pos);
        }

        std::ostringstream ss;
        ss << "Invalid termination in string token at line " << pos.first << " col " << pos.second << ".";
        throw std::runtime_error(ss.str());
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
                // error at escape sequence start
                std::pair<int,int> pos(1,1);
                std::istream::pos_type cur = source.tellg();
                if (cur != (std::istream::pos_type)-1)
                {
                    source.clear(); source.seekg(0);
                    int line = 1, col = 1; char ch;
                    while (source && source.tellg() < cur && source.get(ch)) { if (ch=='\n'){++line; col=1;} else ++col; }
                    pos = {line, col};
                    source.clear(); source.seekg(cur);
                }

                std::ostringstream ss;
                ss << "Invalid termination in string token at line " << pos.first << " col " << pos.second << ".";
                throw std::runtime_error(ss.str());
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
                {
                    std::pair<int,int> pos(1,1);
                    std::istream::pos_type cur = source.tellg();
                    if (cur != (std::istream::pos_type)-1)
                    {
                        source.clear(); source.seekg(0);
                        int line = 1, col = 1; char ch;
                        while (source && source.tellg() < cur && source.get(ch)) { if (ch=='\n'){++line; col=1;} else ++col; }
                        pos = {line, col};
                        source.clear(); source.seekg(cur);
                    }

                    std::ostringstream ss;
                    ss << "Invalid escape in string token at line " << pos.first << " col " << pos.second << ".";
                    throw std::runtime_error(ss.str());
                }
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
        std::pair<int,int> pos(1,1);
        std::istream::pos_type cur = source.tellg();
        if (cur != (std::istream::pos_type)-1)
        {
            source.clear(); source.seekg(0);
            int line = 1, col = 1; char ch;
            while (source && source.tellg() < cur && source.get(ch)) { if (ch=='\n'){++line; col=1;} else ++col; }
            pos = {line, col};
            source.clear(); source.seekg(cur);
        }

        std::ostringstream ss;
        ss << "Invalid end of string token at line " << pos.first << " col " << pos.second << ".";
        throw std::runtime_error(ss.str());
    }

    return result;
}

double tokenize_number(std::istream& source)
{
    double number = 0.0;
    source >> number;

    if (!source)
    {
        // compute approximate position
        std::pair<int,int> pos(1,1);
        std::istream::pos_type cur = source.tellg();
        if (cur != (std::istream::pos_type)-1)
        {
            source.clear(); source.seekg(0);
            int line = 1, col = 1; char ch;
            while (source && source.tellg() < cur && source.get(ch)) { if (ch=='\n'){++line; col=1;} else ++col; }
            pos = {line, col};
            source.clear(); source.seekg(cur);
        }

        std::ostringstream ss;
        ss << "Invalid number token at line " << pos.first << " col " << pos.second << ".";
        throw std::runtime_error(ss.str());
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

    // compute current line/column for the token start
    auto get_pos = [&source]() {
        std::istream::pos_type cur = source.tellg();
        if (cur == (std::istream::pos_type)-1) return std::pair<int,int>(1,1);
        source.clear();
        source.seekg(0);
        int line = 1;
        int col = 1;
        char ch;
        while (source && source.tellg() < cur && source.get(ch))
        {
            if (ch == '\n') { ++line; col = 1; }
            else { ++col; }
        }
        source.clear();
        source.seekg(cur);
        return std::pair<int,int>(line, col);
    };

    auto pos = get_pos();

    char current_symbol = next;
    token.line = pos.first;
    token.column = pos.second;

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
            {
                std::ostringstream ss;
                ss << "Invalid symbol token at line " << token.line << " col " << token.column << ". Make sure JSON file is valid.";
                throw std::runtime_error(ss.str());
            }
        }

        source.get();
        token.data.symbol = current_symbol;
    }

    return source;
}