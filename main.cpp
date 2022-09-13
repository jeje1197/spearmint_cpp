#include <iostream>
#include <memory>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "AstNodes.h"

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
    //std::cout << "Starting lexing." << std::endl;
    Lexer lexer = Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (char *err) {
        std::cout << "Lexing Error: " << err << std::endl;
        return;
    } catch (char const *err) {
        std::cout << "Lexing Error: " << err << std::endl;
        return;
    }

    std::cout << "Lexing complete." << std::endl;
    //for (Token t: tokens) {
    //    std::cout << t << std::endl;
    //}
    //std::cout << "\n----------\n"<< std::endl;


    // Parser
    AstNode ast = nullptr;
    //std::cout << "Starting parsing." << std::endl;
    try {
        Parser parser(tokens);
        ast = parser.parse();
    } catch (char const* err) {
        std::cout << "Parsing Error: " << err << std::endl;
        return;
    }
    //std::cout << "Parsing complete." << std::endl;
    std::cout << ast->toString() << std::endl;

    // Interpreter
}
