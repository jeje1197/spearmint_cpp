#include <iostream>
#include <memory>
#include <fstream>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "AstNodes.h"
#include "Interpreter.h"
#include "Classes.h"
#include "BuiltInFunctions.h"


void run(std::string input);
std::string getFileText(std::string fileName);
void showWelcomeMessage();

Object_sPtr truePrimitive(new Boolean(true));
Object_sPtr falsePrimitive(new Boolean(false));
Object_sPtr nullPrimitive(new Object("Null"));

int main()
{
    showWelcomeMessage();

    while (true) {
        std::string input;
        std::cout << "Spearmint>";
        std::getline(std::cin, input);

        if (input.size() == 0) {
            continue;
        } else if (input == "-e") {
            break;
        } else if (input.find("-r") != std::string::npos) { // Remove "-r "
            int len = (int) input.size();
            int startIndex = (int) input.find("-r") + 3;
            if (startIndex < len) {
                input = input.substr(startIndex, len - startIndex);
                try {
                    input = getFileText(input);
                    run(input);
                } catch (...) {
                    std::cout << "File: '" << input << "' not found.";
                }
            }
            continue;
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
    // std::cout << "Starting lexing." << std::endl;
    Lexer lexer = Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (Exception& err) {
        err.show();
        return;
    }

    if ((int) tokens.size() <= 1) return;

    //std::cout << "Lexing complete." << std::endl;
    //for (Token t: tokens) {
    //    std::cout << t << std::endl;
    //}
    //std::cout << "\n----------\n"<< std::endl;

    // Parser
    //std::cout << "Starting parsing." << std::endl;
    std::vector<AstNode> ast;
    try {
        Parser parser(tokens);
        ast = parser.parse();
    } catch (Exception& err) {
        err.show();
        return;
    }

    // Clean up tokens
    tokens.clear();

    //std::cout << "Parsing complete." << std::endl;
    if (ast.empty()) return;

    //std::cout << "Printing statements" << std::endl;
    //std::cout << "Number of statements: " << ast.size() << std::endl;
    //for (AstNode statement: ast) {
    //    std::cout << statement->toString() << std::endl;
    //}

    // Put vector in AstWrapper
    AstNode programStatements = AstNode(new VectorWrapperNode(ast));

    // Interpreter
    Interpreter interpreter("Console");
    Object_sPtr result = nullPrimitive;
    Context ctx("Base Context", SymbolTable_sPtr(new SymbolTable()));


    ctx.symbol_table->addLocal("true", Object_sPtr(new VariableWrapper(truePrimitive, true)));
    ctx.symbol_table->addLocal("false", Object_sPtr(new VariableWrapper(falsePrimitive, true)));
    ctx.symbol_table->addLocal("null", Object_sPtr(new VariableWrapper(nullPrimitive, true)));
    ctx.symbol_table->addLocal("print", Object_sPtr(new VariableWrapper(printFunction, true)));

    try {
        result = interpreter.visit(programStatements, ctx);
    } catch (Exception& err) {
        err.show();
        return;
    }

    // Print out results
    std::cout << result->toString() << std::endl;
}

std::string getFileText(std::string fileName) {
    std::string fileText, text;
    std::ifstream MyReadFile(fileName);

    while (getline(MyReadFile, text)) {
        fileText += text + "\n";
    }
    return fileText.erase(fileText.length()-1, 1);
}
