#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <sstream>
enum TokenType {
    NEWLINE,
    KEYWORD,
    ID,
    INT,
    DOUBLE,
    STRING,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COLON,
    DOT,
    COMMA,
    SEMICOLON,
    ARROW,
    OP,
    END,
    NULLTYPE
};

class Token {
    public:
        int type;
        std::string value;
        Token() {};
        Token(int type, std::string value);
        Token(int type, char c);
        static Token getNullToken();
};

std::ostream& operator<<(std::ostream& s, const Token& tok);

#endif // TOKEN_H_INCLUDED
