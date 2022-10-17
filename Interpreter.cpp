#include "Interpreter.h"
#include <string>


Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object Interpreter::visit(void* vector_or_node) {
    std::string type = typeid((*)vector_or_node).name();
    std::cout << type << std::endl;
    AstNode node = nullptr;
    if (type.find("Vector") == std::string::npos) {
            std::cout << type << std::endl;
        //node = dynamic_cast<AstNodeBase>((vector_or_node));
    } else {
        std::cout << "vector" << std::endl;
        // visit vector
    }

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
