#include <string>

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
        _NULL
    };

    TokenType type;

    union
    {
        char symbol;
        double number;
    } data;
};