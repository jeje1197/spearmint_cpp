#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>
#include <iostream>
#include <set>
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

class Position {
    public:
        std::string fn;
        int ln;
        int col;

        Position();
        Position(std::string fn);
        void advance(char c);
        Position copy();
        std::string toString();
};

class Token {
    public:
        int type;
        std::string value;
        Position pos;
        Token();
        Token(int type, std::string value);
        Token(int type, std::string value, Position& pos);
        Token(int type, char c);
        Token(int type, char c, Position& pos);
        static Token getNullToken();

        bool matches(int type);
        bool matches(int type, std::string value);
        bool matches(int type, std::set<std::string>& valueSet);
};

std::ostream& operator<<(std::ostream& s, const Token& tok);

#endif // TOKEN_H_INCLUDED
