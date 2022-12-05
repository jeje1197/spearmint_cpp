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

std::vector<AstNode> getAstFromText(std::string input) {
    std::vector<AstNode> ast;

    Lexer lexer = Lexer("Console", input);
    std::vector<Token> tokens;

    try {
        tokens = lexer.getTokens();
    } catch (Exception& err) {
        err.show();
        return ast;
    }

    if ((int) tokens.size() <= 1) return ast;

    // Parser
    std::vector<AstNode> imports;
    try {
        Parser parser(tokens);
        ast = parser.parse();
        imports = parser.importStatements;
    } catch (Exception& err) {
        err.show();
    }

    // Add imported code to current file's ast
    for (int i = (int) imports.size() - 1; i >= 0; i--) {
        std::shared_ptr<ImportNode> importNode = std::static_pointer_cast<ImportNode>(imports.at(i));
        std::string fileText = "";

        try {
            fileText = getFileText(importNode->fileToImport);
        } catch (...) {
            std::cout << "File: '" << input << "' not found.";
        }

        std::vector<AstNode> importedAst = getAstFromText(fileText);
        ast.insert(ast.begin(), importedAst.begin(), importedAst.end());
    }

    return ast;
}

void run(std::string input) {
    std::vector<AstNode> ast = getAstFromText(input);
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

    addBuiltInFunctions(ctx.symbol_table);

    try {
        result = interpreter.visit(programStatements, ctx);
    } catch (Exception& err) {
        err.show();
        return;
    }

    // Print out results
    //std::cout << result->toString() << std::endl;
}

std::string getFileText(std::string fileName) {
    std::string fileText, text;
    std::ifstream MyReadFile(fileName);

    while (getline(MyReadFile, text)) {
        fileText += text + "\n";
    }

    if (fileText.size() > 0) {
        return fileText.erase(fileText.length()-1, 1);
    }
    return "";
}
