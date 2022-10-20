#include <iostream>
#include <memory>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "AstNodes.h"
#include "Interpreter.h"
#include <typeinfo>

void run(std::string input);
void showWelcomeMessage();

int main()
{
    showWelcomeMessage();

    while (true) {
        std::string input;
        std::cout << "Spearmint>";
        std::getline(std::cin, input);
        //std::cout << "Input: " << input << std::endl;

        if (input.size() == 0) {
            continue;
        } else if (input == "-e" || input == "-exit") {
            break;
        }

        run(input);
    }

    return 0;
}

void showWelcomeMessage() {
    std::cout << "----- Spearmint Interpreter -----" << std::endl;
    std::cout << "Type '-e' or '-exit' to close the shell.\nType -help to see a list of available commands.\n" << std::endl;
}

void run(std::string input) {
    // Lexer
    std::cout << "Starting lexing." << std::endl;
    Lexer lexer = Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (Exception& err) {
        err.show();
        return;
    }

    std::cout << "Lexing complete." << std::endl;
    for (Token t: tokens) {
        std::cout << t << std::endl;
    }
    //std::cout << "\n----------\n"<< std::endl;

    // Parser
    std::vector<AstNode> ast;
    std::cout << "Starting parsing." << std::endl;
    try {
        Parser parser(tokens);
        ast = parser.parse();
    } catch (Exception& err) {
        err.show();
        return;
    }

    std::cout << "Parsing complete." << std::endl;
    if (ast.empty()) {
        std::cout << "Ast is empty." << std::endl;
        return;
    }

    std::cout << "Printing statements" << std::endl;
    std::cout << "Number of statements: " << ast.size() << std::endl;
    for (AstNode statement: ast) {
        std::cout << statement->toString() << std::endl;
    }

    // Put vector in AstWrapper
    AstNode programStatements = AstNode(new VectorWrapperNode(ast));

    std::cout << "Starting interpreter." << std::endl;

    // Interpreter
    Interpreter interpreter("Console");
    Object_sPtr result = Object::NullType();
    try {
        interpreter.visit(programStatements);
    } catch (Exception& err) {
        err.show();
        return;
    }

    // Print out results
    if (result != Object::NullType()) {
        std::cout << result->toString() << std::endl;
    }

}
