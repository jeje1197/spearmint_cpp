#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>
#include <sstream>
#include <iostream>
#include <set>

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

        bool matches(int type);
        bool matches(int type, std::string value);
        bool matches(int type, std::set<std::string>& valueSet);
};

std::ostream& operator<<(std::ostream& s, const Token& tok);

#endif // TOKEN_H_INCLUDED
