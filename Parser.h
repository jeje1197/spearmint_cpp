#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Token.h"
#include "AstNodes.h"
#include "Exception.h"

#include <string>
#include <vector>
#include <set>

class Parser {
    public:
        std::vector<Token> *tokens;
        int index;
        Token curTok;

        std::vector<AstNode> importStatements;

        Parser(std::vector<Token>& tokens);

        bool hasNext(int steps);
        Token getNext();
        Token lookAhead(int steps);
        void skipSemis();

        // Parse Entry Point
        std::vector<AstNode> parse();

        // Statement Parsing
        std::vector<AstNode> statements(int ENDTYPE);
        AstNode statement();

        // Import Statement Parsing
        void importStatement();

        // Variable Action Parsing
        AstNode varDeclaration();
        AstNode varAssign();

        // If Statements
        AstNode ifStatement();

        // Loop Parsing
        AstNode forStatement();
        AstNode whileStatement();

        // Function Parsing (PassableFunctions?)
        AstNode functionDef();
        AstNode constructorStatement();
        AstNode call();

        // Helper for parsing function calls
        AstNode functionCall(AstNode atomNode);

        AstNode returnStatement();
        AstNode breakStatement();
        AstNode continueStatement();

        // Structure Parsing
        AstNode classDef();

        // Expression Parsing
        AstNode expr();
        AstNode not_expr();
        AstNode comp_expr();
        AstNode arith_expr();
        AstNode term();
        AstNode power();
        AstNode atom();

        typedef AstNode (Parser::* ParserFunction)();

        AstNode binOp(ParserFunction func1, std::set<std::string> ops, ParserFunction func2);

};



#endif // PARSER_H_INCLUDED
