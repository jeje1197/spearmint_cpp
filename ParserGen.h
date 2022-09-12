#ifndef PARSERGEN_H_INCLUDED
#define PARSERGEN_H_INCLUDED

#include "Token.h"

using std::string;

class ParserGen {
    string
};

class CheckFor {
    string type, value;
    CheckFor(string type, string value) {
        this->type = type;
        this->value = value;
    }

    bool execute(Token tok) {
        return type == tok.type && value == tok.value;
    }
};


#endif // PARSERGEN_H_INCLUDED
