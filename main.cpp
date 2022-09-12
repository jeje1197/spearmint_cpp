#include <iostream>
#include <sstream>
#include <memory>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "AstNodes.h"

using std::shared_ptr;

int main()
{
    std::string input = " 10+4+3.6sdfghjhgfd567564456...////bjklbv";

    // Lexer
    Lexer* lexer = new Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer->getTokens();
    } catch (char const *err) {
        std::cout << "Lexing Error: "<< err << std::endl;
        return 0;
    }

    std::cout << "After lex" << std::endl;

    for (Token t: tokens) {
        std::cout << t << std::endl;
    }
    std::cout << "\n----------\n"<< std::endl;

    AstNode* ast = nullptr;
    try {
        Parser parser(tokens);
        ast = parser.parse();
    } catch (char const* err) {
        std::cout << "Parsing Error: " << err << std::endl;
        return 0;
    }
    std::cout << ast->toString();

    return 0;
}
