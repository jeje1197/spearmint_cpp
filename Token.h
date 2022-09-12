#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

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
        Token() {}
        Token(int type, std::string value) {
            this->type = type;
            this->value = value;
        }
        Token(int type, char c) {
            this->type = type;
            this->value = c;
        }
};

Token NULLTOK(NULLTYPE, "NULL");

std::ostream& operator<<(std::ostream& s, const Token& tok){
    s << "(" << tok.type << ", " << tok.value << ")";
    return s;
}

#endif // TOKEN_H_INCLUDED
