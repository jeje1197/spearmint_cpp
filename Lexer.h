#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include "Token.h"
#include <vector>
#include <ctype.h>
#include <unordered_map>

class Lexer {
    public:
        std::string fileName;
        std::string text;
        int index;
        char curChar;

        Lexer(std::string fileName, std::string text) {
            this->fileName = fileName;
            this->text = text;
            this->index = 0;
            this->curChar = text.at(0);
        }

        bool hasNext() {
            return index + 1 < (int) text.length();
        }

        char getNext() {
            if (hasNext()) {
                index++;
                curChar = text.at(index);
            } else {
                curChar = '\0';
            }
            return curChar;
        }

        char lookAhead(int steps) {
            if ((this->index + steps) < (int)text.length()) {
                return this->text.at(this->index + steps);
            } else {
                return '_';
            }
        }

        std::vector<Token>* getTokens() {
            std::vector<Token> tokens;
            std::string operators = "+-*/^%=<>!";

            while (curChar != '\0') {
                std::string next2chars = std::string(1, curChar) + lookAhead(1);

                if (curChar == ' ' || curChar == '\t') {
                    // Skip whitespace
                } else if (curChar == '\n') { // Newlines
                    Token token(NEWLINE, curChar);
                    tokens.push_back(token);
                } else if (isdigit(curChar)) { // Numbers
                    std::string number(1, curChar);
                    this->getNext();

                    int decimal_count = 0;
                    while (curChar != '\0' && (isdigit(curChar) || curChar == '.')) {
                        number += curChar;
                        if (curChar == '.') {
                            if (decimal_count == 1) break;
                            decimal_count++;
                        }
                        this->getNext();
                    }


                    if (decimal_count == 0) {
                        tokens.push_back(Token(INT, number));
                    } else {
                        tokens.push_back(Token(DOUBLE, number));
                    }

                    continue;
                } else if (curChar == '"') { // Strings
                    this->getNext();

                    std::string str;
                    while (curChar != '"') {

                        if (curChar == '\0') {
                            throw "Unterminated string";
                        }
                        str += curChar;
                        this->getNext();
                    }

                    tokens.push_back(Token(STRING, str));
                } else if (next2chars == "!=" || next2chars == "<=" || next2chars == ">=" ||
                           next2chars == "&&" || next2chars == "!=") { // Operators (2 char)
                    tokens.push_back(Token(OP, next2chars));
                    this->getNext();
                } else if (operators.find(curChar) != std::string::npos) { // Operators (1 char)
                    tokens.push_back(Token(OP, curChar));
                } else if (curChar == '.') {
                    tokens.push_back(Token(DOT, curChar));
                } else if (curChar == ';') {
                    tokens.push_back(Token(SEMICOLON, curChar));
                } else if (curChar == '(') {
                    tokens.push_back(Token(LPAREN, curChar));
                } else if (curChar == ')') {
                    tokens.push_back(Token(RPAREN, curChar));
                } else if (curChar == '{') {
                    tokens.push_back(Token(LBRACE, curChar));
                } else if (curChar == '}') {
                    tokens.push_back(Token(RBRACE, curChar));
                } else if (curChar == '[') {
                    tokens.push_back(Token(LBRACKET, curChar));
                } else if (curChar == ']') {
                    tokens.push_back(Token(RBRACKET, curChar));
                } else {
                    throw "Invalid char: " + curChar;
                }
                this->getNext();
            }

            tokens.push_back(Token(END, "END"));
            return &tokens;
        }
};


#endif // LEXER_H_INCLUDED
