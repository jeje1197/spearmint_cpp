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
    std::string sample = " 10+3+16";

    // Lexer
    Lexer lexer("Console", sample);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (char const *err) {
        std::cout << "Lexing Error: "<< err << std::endl;
        return 0;
    }

    std::cout << "After lex";

    // Print out list of token representations
    std::ostringstream out;
    std::cout << tokens->at(0);

    std::cout << "Tokens:\n" << std::string(out.str()) + "\n----------\n"<< std::endl ;

    AstNode* ast;

    try {
        //Parser parser(tokens);
        //ast = parser.parse();
    } catch (char const* err) {
        std::cout << "Parsing Error: " << err << std::endl;
        return 0;
    }
    //std::cout << ast-> toString();

    return 0;
}
