#include "Interpreter.h"
#include <string>

#include <iostream>
#include <memory>

Object_sPtr Null_sPtr = Object::NullType();

Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object_sPtr Interpreter::visit(AstNode node) {
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
    } else if (type == "VarDeclarationNode") {
        return visit_VarDeclarationNode(node);
    } else if (type == "VarAssignNode") {
        return visit_VarAssignNode(node);
    } else {
        throw Exception("No visit_" + node->type + " method defined.");
    }

    return Null_sPtr;
}

Object_sPtr Interpreter::visit_VectorWrapperNode(AstNode node){
    std::vector<AstNode> v = std::static_pointer_cast<VectorWrapperNode>(node)->getVector();

    for (AstNode a: v) {
        visit(a);
    }
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_NumberNode(AstNode node){
    std::shared_ptr<NumberNode> numNode = std::static_pointer_cast<NumberNode>(node);
    return Object_sPtr(new Number(std::stof(numNode->value)));
}

Object_sPtr Interpreter::visit_StringNode(AstNode node){
    std::shared_ptr<StringNode> strNode = std::static_pointer_cast<StringNode>(node);
    return Object_sPtr(new String(strNode->value));
}

Object_sPtr Interpreter::visit_VarDeclarationNode(AstNode node){
    std::shared_ptr<VarDeclarationNode> varNode = std::static_pointer_cast<VarDeclarationNode>(node);
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_VarAssignNode(AstNode node){
    std::shared_ptr<VarDeclarationNode> varNode = std::static_pointer_cast<VarDeclarationNode>(node);
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_VarAccessNode(AstNode node){
    std::shared_ptr<VarDeclarationNode> varNode = std::static_pointer_cast<VarDeclarationNode>(node);
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_UnaryOpNode(AstNode node){
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_BinOpNode(AstNode node){
    // Cast shared_ptr<AstNode> to shared_ptr<BinOpNode>
    std::shared_ptr<BinOpNode> binOpNode = std::static_pointer_cast<BinOpNode>(node);

    // Use unique ptr for virtual calls
    Object_sPtr left = visit(binOpNode->left);
    Object_sPtr right = visit(binOpNode->right);

    Object_sPtr res = nullptr;
    if (binOpNode->op.compare("+") == 0) {
        res = left->add(right);
    } else if (binOpNode->op.compare("-") == 0) {

    } else if (binOpNode->op.compare("*") == 0) {

    } else if (binOpNode->op.compare("/") == 0) {

    } else {

    }

    return res;
}

