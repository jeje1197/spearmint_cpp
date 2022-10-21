#include "Lexer.h"

Lexer::Lexer(std::string fileName, std::string text) {
    this->fileName = fileName;
    this->text = text;
    this->index = 0;
    this->curChar = text.at(0);
    this->curPos = Position(fileName);
}

bool Lexer::hasNext() {
    return this->index + 1 < (int) text.length();
}

char Lexer::getNext() {
    if (hasNext()) {
        this->curPos.advance(curChar);
        index++;
        this->curChar = this->text.at(index);
    } else {
        this->curChar = '\0';
    }
    return curChar;
}

char Lexer::lookAhead(int steps) {
    if ((this->index + steps) < (int) text.length()) {
        return this->text.at(this->index + steps);
    } else {
        return '\0';
    }
}

std::vector<Token> Lexer::getTokens() {
    std::vector<Token> tokens;
    std::string operators = "+-*/^%=<>!";
    std::set<std::string> keywords = {"var", "if", "else", "for", "while", "fun", "class"};

    while (curChar != '\0') {
        std::string next2chars = std::string(1, curChar) + lookAhead(1);

        if (curChar == ' ' || curChar == '\t' || curChar == '\n') {
            // Skip whitespace
        } else if (curChar == '#') { // Comments
            while (curChar != '\n' && curChar != '\0') {
                getNext();
            }
        } else if (isalpha(curChar) || curChar == '_') { // Keywords and Identifiers
            Position startPos = curPos.copy();
            std::string str(1, curChar);
            getNext();

            while (hasNext() && (isalnum(curChar) || curChar == '_')) {
                str += curChar;
                getNext();
            }

            if (keywords.find(str)!= keywords.end()) {
                tokens.push_back(Token(KEYWORD, str, startPos));
            } else {
                tokens.push_back(Token(ID, str, startPos));
            }
            continue;
        } else if (isdigit(curChar)) { // Numbers
            Position startPos = curPos.copy();
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
            Position startPos = curPos.copy();
            this->getNext();

            std::string str;
            while (curChar != '"') {

                if (curChar == '\0') {
                    throw Exception("Unterminated string " + startPos.toString());
                }
                str += curChar;
                this->getNext();
            }

            tokens.push_back(Token(STRING, str, startPos));
        } else if (next2chars == "!=" || next2chars == "==" || next2chars == "<=" || next2chars == ">=" ||
                   next2chars == "&&" || next2chars == "||") { // Operators (2 char)
            tokens.push_back(Token(OP, next2chars, curPos));
            this->getNext();
        } else if (operators.find(curChar) != std::string::npos) { // Operators (1 char)
            tokens.push_back(Token(OP, curChar, curPos));
        } else if (curChar == '.') {
            tokens.push_back(Token(DOT, curChar, curPos));
        } else if (curChar == ',') {
            tokens.push_back(Token(COMMA, curChar, curPos));
        } else if (curChar == ';') {
            tokens.push_back(Token(SEMICOLON, curChar, curPos));
        } else if (curChar == '(') {
            tokens.push_back(Token(LPAREN, curChar, curPos));
        } else if (curChar == ')') {
            tokens.push_back(Token(RPAREN, curChar, curPos));
        } else if (curChar == '{') {
            tokens.push_back(Token(LBRACE, curChar, curPos));
        } else if (curChar == '}') {
            tokens.push_back(Token(RBRACE, curChar, curPos));
        } else if (curChar == '[') {
            tokens.push_back(Token(LBRACKET, curChar, curPos));
        } else if (curChar == ']') {
            tokens.push_back(Token(RBRACKET, curChar, curPos));
        } else {
            throw Exception("Invalid Char: " + std::string(1, curChar) + curPos.toString());
        }
        this->getNext();
    }

    tokens.push_back(Token(END, "END"));
    return tokens;
}
