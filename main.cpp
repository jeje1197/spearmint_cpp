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
    std::string input = " 10+4+3.6-3434+5675+432-4637-23478+66-6637";

    // Lexer
    std::cout << "Starting lexing." << std::endl;
    Lexer lexer = Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (char const *err) {
        std::cout << "Lexing Error: "<< err << std::endl;
        return 0;
    }

    std::cout << "Lexing complete." << std::endl;
    for (Token t: tokens) {
        std::cout << t << std::endl;
    }
    std::cout << "\n----------\n"<< std::endl;


    shared_ptr<AstNode> ast = nullptr;
    std::cout << "Starting parsing." << std::endl;
    try {
        Parser parser(tokens);
        ast = parser.parse();
    } catch (char const* err) {
        std::cout << "Parsing Error: " << err << std::endl;
        return 0;
    }
    std::cout << "Parsing complete." << std::endl;
    std::cout << ast->toString() << std::endl;

    return 0;
}
