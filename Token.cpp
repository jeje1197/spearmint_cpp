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

std::ostream& operator<<(std::ostream& s, const Token& tok){
    s << "(" << tok.type << ", " << tok.value << ")";
    return s;
}
