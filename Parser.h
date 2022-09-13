#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Token.h"
#include "AstNodes.h"
#include <vector>
#include <set>

class Parser {
    public:
        std::vector<Token> *tokens;
        int index;
        Token curTok;

        Parser(std::vector<Token>& tokens);

        bool hasNext(int steps);
        void getNext();
        Token lookAhead(int steps);
        void skipSemis();

        // Parse Entry Point
        AstNode parse();

        // Statement Parsing
        std::vector<AstNode> statements();
        AstNode statement();


        // Variable Action Parsing
        AstNode varDeclaration();
        AstNode varAssign();

        // Expression Parsing
        AstNode expr();
        AstNode comp_expr1();
        AstNode comp_expr2();
        AstNode arith_expr();
        AstNode term();
        AstNode power();
        AstNode atom();

};

#endif // PARSER_H_INCLUDED
