#include "Interpreter.h"
#include <string>


Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object Interpreter::visit(AstNode node) {
    std::string type = node->type;

    if (type == "VectorWrapperNode") {
        return visit_VectorWrapperNode(node);
    } else if (type == "NumberNode") {
        return visit_NumberNode(node);
    } else if (type == "StringNode") {
        return visit_StringNode(node);
    } else if (type == "UnaryOpNode") {
        return visit_UnaryOpNode(node);
    } else if (type == "BinOpNode") {
        return visit_BinOpNode(node);
    //} else if (type == "VarDeclarationNode") {
    //   return visit_VarDeclarationNode(node);
    //} else if (type == "VarAssignNode") {
    //    return visit_VarAssignNode(node);
    } else {
        throw Exception("No visit_" + node->type + " method defined.");
    }

    return Object();
}

Object Interpreter::visit_VectorWrapperNode(AstNode node){
    return NullType();
}

Number Interpreter::visit_NumberNode(AstNode node){
    return Number();
}

String Interpreter::visit_StringNode(AstNode node){
    return String();
}

Object Interpreter::visit_UnaryOpNode(AstNode node){
    return Number();
}

Object Interpreter::visit_BinOpNode(AstNode node){
    return Number();
}

