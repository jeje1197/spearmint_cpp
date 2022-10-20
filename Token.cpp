#include "Token.h"

Position::Position() {}

Position::Position(std::string fn) {
    this->fn = fn;
    this->ln = 0;
    this->col = 0;
}

void Position::advance(char c) {
    this->col++;
    if (c == '\n') {
        this->ln++;
        this->col = 0;
    }
}

Position Position::copy() {
    Position p = Position();
    p.fn = this->fn;
    p.ln = this->ln;
    p.col = this->col;
    return p;
}
std::string Position::toString() {
    return "at Line: " + std::to_string(ln) + " Col: " + std::to_string(col) + " in File: '" +
    fn + "'";
}

Token::Token() {
    this->type = -1;
    this->value = "NULLTOK";
}

Token::Token(int type, std::string value) {
    this->type = type;
    this->value = value;
}

Token::Token(int type, std::string value, Position& pos) {
    this->type = type;
    this->value = value;
    this->pos = pos.copy();
}

Token::Token(int type, char c) {
    this->type = type;
    this->value = c;
}

Token::Token(int type, char c, Position& pos) {
    this->type = type;
    this->value = c;
    this->pos = pos.copy();
}

Token NULLTOK(NULLTYPE, "NULL");
Token Token::getNullToken() {return NULLTOK;}

bool Token::matches(int type) {
    return this->type == type;
}

bool Token::matches(int type, std::string value) {
    return this->type == type && this->value == value;
}

bool Token::matches(int type, std::set<std::string>& valueSet) {
    return this->type == type && (valueSet.find(this->value) != valueSet.end());
}

std::ostream& operator<<(std::ostream& s, const Token& tok){
    s << "(" << tok.type << ", " << tok.value << ")";
    return s;
}
