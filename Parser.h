#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Token.h"
#include "AstNodes.h"
#include <vector>
#include <unordered_map>
#include <memory>

class Parser {
    public:
        std::vector<Token> *tokens;
        int index;
        Token curTok;

        Parser(std::vector<Token>& tokens);

        bool hasNext(int steps);
        void getNext();
        Token lookAhead(int steps);

        std::shared_ptr<AstNode> parse();

        // Statement Parsing
        std::shared_ptr<AstNode> statement();


        // Variable Actions
        std::shared_ptr<AstNode> varDeclaration();
        std::shared_ptr<AstNode> varAssign();
        std::shared_ptr<AstNode> varAccess();

        // Expression Parsing
        std::shared_ptr<AstNode> expr();
        std::shared_ptr<AstNode> comp_expr1();
        std::shared_ptr<AstNode> comp_expr2();
        std::shared_ptr<AstNode> arith_expr();
        std::shared_ptr<AstNode> term();
        std::shared_ptr<AstNode> power();
        std::shared_ptr<AstNode> atom();

};

#endif // PARSER_H_INCLUDED
