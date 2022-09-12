#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <string>
#include <vector>
#include "Token.h"

class Lexer {
    public:
        std::string fileName;
        std::string text;
        int index;
        char curChar;

        Lexer(std::string fileName, std::string text);

        bool hasNext();
        char getNext();
        char lookAhead(int steps);
        std::vector<Token> getTokens();
};


#endif // LEXER_H_INCLUDED
