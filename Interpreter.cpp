#include "Interpreter.h"
#include <string>

#include <iostream>
#include <memory>

Object_sPtr Null_sPtr = Object::NullType();

Interpreter::Interpreter() {}
Interpreter::Interpreter(std::string fileName) {
	this->fileName = fileName;
}


Object_sPtr Interpreter::visit(AstNode node, Context& ctx) {
    std::string type = node->type;

    if (type == "VectorWrapperNode") {
        return visit_VectorWrapperNode(node, ctx);
    } else if (type == "NumberNode") {
        return visit_NumberNode(node, ctx);
    } else if (type == "StringNode") {
        return visit_StringNode(node, ctx);
    } else if (type == "UnaryOpNode") {
        return visit_UnaryOpNode(node, ctx);
    } else if (type == "BinOpNode") {
        return visit_BinOpNode(node, ctx);
    } else if (type == "VarDeclarationNode") {
        return visit_VarDeclarationNode(node, ctx);
    } else if (type == "VarAssignNode") {
        return visit_VarAssignNode(node, ctx);
    } else if (type == "VarAccessNode") {
        return visit_VarAccessNode(node, ctx);
    } else if (type == "IfNode") {
        return visit_IfNode(node, ctx);
    } else if (type == "ForNode") {
        return visit_ForNode(node, ctx);
    } else if (type == "WhileNode") {
        return visit_WhileNode(node, ctx);
    } else {
        throw Exception("No visit_" + node->type + " method defined.");
    }

    return Null_sPtr;
}

Object_sPtr Interpreter::visit_VectorWrapperNode(AstNode node, Context& ctx){
    std::vector<AstNode> v = std::static_pointer_cast<VectorWrapperNode>(node)->getVector();
    Object_sPtr retList = Object_sPtr(new List());
    for (AstNode a: v) {
        retList->add(visit(a, ctx));
    }

    if (retList->getSizeInternal() == 1) {
        return retList->getInternal(0);
    }
    return retList;
}

Object_sPtr Interpreter::visit_NumberNode(AstNode node, Context& ctx){
    std::shared_ptr<NumberNode> numNode = std::static_pointer_cast<NumberNode>(node);
    return Object_sPtr(new Number(std::stof(numNode->value)));
}

Object_sPtr Interpreter::visit_StringNode(AstNode node, Context& ctx){
    std::shared_ptr<StringNode> strNode = std::static_pointer_cast<StringNode>(node);
    return Object_sPtr(new String(strNode->value));
}

Object_sPtr Interpreter::visit_VarDeclarationNode(AstNode node, Context& ctx){
    std::shared_ptr<VarDeclarationNode> varNode = std::static_pointer_cast<VarDeclarationNode>(node);
    if (ctx.symbol_table->containsLocalKey(varNode->varName)) {
        throw Exception("'" + varNode->varName + "' is already in scope.");
    }

    Object_sPtr value = visit(varNode->exprNode, ctx);
    Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(value));
    ctx.symbol_table->addLocal(varNode->varName, varWrapper);
    return value;
}

Object_sPtr Interpreter::visit_VarAssignNode(AstNode node, Context& ctx){
    std::shared_ptr<VarAssignNode> varNode = std::static_pointer_cast<VarAssignNode>(node);
    if (!ctx.symbol_table->containsKeyAnywhere(varNode->varName)) {
        throw Exception("'" + varNode->varName + "' has not been declared.");
    }

    Object_sPtr varWrapper = ctx.symbol_table->get(varNode->varName);
    if (varWrapper->isConstant()) {
        throw Exception("Value cannot be reassigned. Variable '" + varNode->varName + "' is declared as constant.");
    }
    Object_sPtr value = visit(varNode->exprNode, ctx);
    varWrapper->storeObject(value);
    return value;
}

Object_sPtr Interpreter::visit_VarAccessNode(AstNode node, Context& ctx){
    std::shared_ptr<VarAccessNode> varNode = std::static_pointer_cast<VarAccessNode>(node);
    if (!ctx.symbol_table->containsKeyAnywhere(varNode->varName)) {
        throw Exception("'" + varNode->varName + "' has not been declared.");
    }

    Object_sPtr varWrapper = ctx.symbol_table->get(varNode->varName);
    return varWrapper->getObject();
}

Object_sPtr Interpreter::visit_UnaryOpNode(AstNode node, Context& ctx){
    std::shared_ptr<UnaryOpNode> unaryOpNode = std::static_pointer_cast<UnaryOpNode>(node);
    Object_sPtr res = visit(unaryOpNode->exprNode, ctx);

    if (unaryOpNode->op.compare("-") == 0) {
        res = res->mul(Object_sPtr(new Number(-1)));
    } else if (unaryOpNode->op.compare("!") == 0) {
        res = res->notted();
    }
    return res;
}

Object_sPtr Interpreter::visit_BinOpNode(AstNode node, Context& ctx){
    // Cast shared_ptr<AstNode> to shared_ptr<BinOpNode>
    std::shared_ptr<BinOpNode> binOpNode = std::static_pointer_cast<BinOpNode>(node);

    // Use unique ptr for virtual calls
    Object_sPtr left = visit(binOpNode->left, ctx);
    Object_sPtr right = visit(binOpNode->right, ctx);

    Object_sPtr res = Null_sPtr;
    if (binOpNode->op.compare("+") == 0) {
        res = left->add(right);
    } else if (binOpNode->op.compare("-") == 0) {
        res = left->sub(right);
    } else if (binOpNode->op.compare("*") == 0) {
        res = left->mul(right);
    } else if (binOpNode->op.compare("/") == 0) {
        res = left->div(right);
    } else if (binOpNode->op.compare("^") == 0) {
        res = left->pow(right);
    } else if (binOpNode->op.compare("%") == 0) {
        res = left->mod(right);
    } else if (binOpNode->op.compare("<") == 0) {
        res = left->compare_lt(right);
    } else if (binOpNode->op.compare(">") == 0) {
        res = left->compare_gt(right);
    } else if (binOpNode->op.compare("<=") == 0) {
        res = left->compare_lte(right);
    } else if (binOpNode->op.compare(">=") == 0) {
        res = left->compare_gte(right);
    } else if (binOpNode->op.compare("==") == 0) {
        res = left->compare_ee(right);
    } else if (binOpNode->op.compare("!=") == 0) {
        res = left->compare_ne(right);
    } else if (binOpNode->op.compare("&&") == 0) {
        res = left->anded_by(right);
    } else if (binOpNode->op.compare("||") == 0) {
        res = left->ored_by(right);
    } else {
        res = left->pow(right);
    }

    return res;
}

Object_sPtr Interpreter::visit_IfNode(AstNode node, Context& ctx) {
    std::shared_ptr<IfNode> ifNode = std::static_pointer_cast<IfNode>(node);
    Context newCtx = ctx.generateNewContext("If statement in " + ctx.name);
    for (int i = 0; i < (int) ifNode->caseConditions.size(); i++) {
        Object_sPtr cond = visit(ifNode->caseConditions.at(i), ctx);
        if (cond->is_true()) {
            Object_sPtr res = visit(AstNode(new VectorWrapperNode(ifNode->caseStatements.at(i))), newCtx);
            return res;
        }
    }

    AstNode vw = AstNode(new VectorWrapperNode(ifNode->elseCaseStatements));
    Object_sPtr res = visit(vw, newCtx);
    return res;
}

Object_sPtr Interpreter::visit_ForNode(AstNode node, Context& ctx) {
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_WhileNode(AstNode node, Context& ctx) {
    return Null_sPtr;
}

