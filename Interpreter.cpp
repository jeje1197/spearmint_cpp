#include "Interpreter.h"
#include <string>


Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object Interpreter::visit(AstNode node) {
    std::string type = node->type;

    if (type == "NumberNode") {

        std::cout << "Number" << std::endl;
        return visit_NumberNode(node);
    } else if (type == "StringNode") {

    } else if (type == "NumberNode") {

    } else if (type == "NumberNode") {

    }

    return Object();
}

Number Interpreter::visit_NumberNode(AstNode node){
    return Number();
}
