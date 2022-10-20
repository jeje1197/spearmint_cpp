#include "Interpreter.h"
#include <string>

#include <iostream>
#include <memory>


Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object_uPtr Interpreter::visit(AstNode node) {
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

    return Object_uPtr(new NullType());
}

Object_uPtr Interpreter::visit_VectorWrapperNode(AstNode node){
    std::vector<AstNode> v = std::static_pointer_cast<VectorWrapperNode>(node)->getVector();
    for (AstNode a: v) {
        visit(a);
    }
    return Object_uPtr(new NullType());
}

Object_uPtr Interpreter::visit_NumberNode(AstNode node){
    std::shared_ptr<NumberNode> numNode = std::static_pointer_cast<NumberNode>(node);
    return Object_uPtr(new Number(std::stof(numNode->value)));
}

Object_uPtr Interpreter::visit_StringNode(AstNode node){
    std::shared_ptr<StringNode> strNode = std::static_pointer_cast<StringNode>(node);
    return Object_uPtr(new String(strNode->value));
}

Object_uPtr Interpreter::visit_UnaryOpNode(AstNode node){
    return Object_uPtr(new NullType());
}

Object_uPtr Interpreter::visit_BinOpNode(AstNode node){
    // Cast Node
    std::shared_ptr<BinOpNode> binOpNode = std::static_pointer_cast<BinOpNode>(node);

    // Use unique ptr for vitual calls
    Object_uPtr left = visit(binOpNode->left);
    Object_uPtr right = visit(binOpNode->right);

    if (binOpNode->op.compare("+") == 0) {
        std::cout << left->add(right)->toString();

    } else if (binOpNode->op.compare("-") == 0) {

    } else if (binOpNode->op.compare("*") == 0) {

    } else if (binOpNode->op.compare("/") == 0) {
        ;
    }
    return Object_uPtr(new NullType());
}

