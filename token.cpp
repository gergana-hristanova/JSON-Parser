#include "statement.hpp"

struct Token
{
    enum class TokenType
    {
        // structural tokens:
        LEFT_BRACE,    //  {
        RIGHT_BRACE,   //  }
        LEFT_BRACKET,  //  (
        RIGHT_BRACKET, //  )
        COLON,         //  :
        COMMA,         //  ,

        // value tokens:
        NUMBER,
        STRING,
        BOOLEAN,
        _NULL,
        ARRAY,
        OBJECT
    };

    TokenType type;

    union
    {
        Statement* statement;
        char symbol;
    } data;
    
};