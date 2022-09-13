#include "Token.h"


Token::Token(int type, std::string value) {
    this->type = type;
    this->value = value;
}

Token::Token(int type, char c) {
    this->type = type;
    this->value = c;
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
