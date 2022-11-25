#include "Interpreter.h"
#include "Exception.h"
#include "Context.h"
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
    } else if (type == "FunctionDefNode") {
        return visit_FunctionDefNode(node, ctx);
    } else if (type == "FunctionCallNode") {
        return visit_FunctionCallNode(node, ctx);
    } else if (type == "ReturnNode") {
        return visit_ReturnNode(node, ctx);
    } else if (type == "BreakNode") {
        return visit_BreakNode(node, ctx);
    } else if (type == "ContinueNode") {
        return visit_ContinueNode(node, ctx);
    } else if (type == "ClassDefNode") {
        return visit_ClassDefNode(node, ctx);
    } else if (type == "ConstructorCallNode") {
        return visit_ConstructorCallNode(node, ctx);
    } else if (type == "AttributeAccessNode") {
        return visit_AttributeAccessNode(node, ctx);
    } else if (type == "AttributeAssignNode") {
        return visit_AttributeAssignNode(node, ctx);
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
        if (this->should_return) {
            return this->return_value;
        } else if (this->should_break || this->should_continue) {
            return Null_sPtr;
        }
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
    Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(value, varNode->isConstant));
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
    std::shared_ptr<BinOpNode> binOpNode = std::static_pointer_cast<BinOpNode>(node);

    // Use shared ptr for virtual calls
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
    std::shared_ptr<ForNode> forNode = std::static_pointer_cast<ForNode>(node);
    Context initCtx = ctx.generateNewContext("For loop initializer");
    visit(forNode->initStatement, initCtx);

    Object_sPtr retList = Object_sPtr(new List());
    while (visit(forNode->condNode, initCtx)->is_true()) {
        Context iterCtx = initCtx.generateNewContext("For loop iteration");
        retList->add(visit(AstNode(new VectorWrapperNode(forNode->statements)), iterCtx));
        if (this->should_break) {
            this->should_break = false;
            break;
        } else if (this->should_continue) {
            this->should_continue = false;
        }
        visit(forNode->updateStatement, iterCtx);
    }

    if (retList->getSizeInternal() == 1) {
        return retList->getInternal(0);
    }
    return retList;
}

Object_sPtr Interpreter::visit_WhileNode(AstNode node, Context& ctx) {
    std::shared_ptr<WhileNode> whileNode = std::static_pointer_cast<WhileNode>(node);

    Object_sPtr retList = Object_sPtr(new List());
    while (visit(whileNode->condNode, ctx)->is_true()) {
        Context iterCtx = ctx.generateNewContext("While loop iteration");
        retList->add(visit(AstNode(new VectorWrapperNode(whileNode->statements)), iterCtx));
        if (this->should_break) {
            this->should_break = false;
            break;
        } else if (this->should_continue) {
            this->should_continue = false;
        }
    }

    if (retList->getSizeInternal() == 1) {
        return retList->getInternal(0);
    }
    return retList;
}

Object_sPtr Interpreter::visit_FunctionDefNode(AstNode node, Context& ctx) {
    std::shared_ptr<FunctionDefNode> funDefNode = std::static_pointer_cast<FunctionDefNode>(node);
    if (ctx.symbol_table->containsLocalKey(funDefNode->name)) {
        throw Exception("Cannot define function. '" + funDefNode->name + "' is already in scope.");
    }


    Object_sPtr newFunction(new Function(funDefNode->name, funDefNode->argNames, funDefNode->statements));
    Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(newFunction, false));
    ctx.symbol_table->addLocal(funDefNode->name, varWrapper);
    return newFunction;
}

Object_sPtr Interpreter::visit_FunctionCallNode(AstNode node, Context& ctx) {
    std::shared_ptr<FunctionCallNode> funCallNode = std::static_pointer_cast<FunctionCallNode>(node);

    std::shared_ptr<Function> functionObj = std::static_pointer_cast<Function>(visit(funCallNode->nodeToCall, ctx));
    functionObj->isCallable();
    functionObj->checkNumArgs(funCallNode->argNodes);

    Context funCtx = ctx.generateNewContext("Function '" + functionObj->name + "'");
    for (int i = 0; i < (int) functionObj->argNames.size(); i++) {
        Object_sPtr arg_value = visit(funCallNode->argNodes.at(i), ctx);
        Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(arg_value));
        funCtx.symbol_table->addLocal(functionObj->argNames.at(i), varWrapper);
    }

    if (functionObj->isBuiltIn()) {
        return functionObj->executeWrapper(&funCtx);
    }

    Object_sPtr res = visit(AstNode(new VectorWrapperNode(functionObj->statements)), funCtx);

    // Clear return flag & value
    this->should_return = false;
    this->return_value = Null_sPtr;

    return res;
}

Object_sPtr Interpreter::visit_ReturnNode(AstNode node, Context& ctx){
    std::shared_ptr<ReturnNode> returnNode = std::static_pointer_cast<ReturnNode>(node);
    this->return_value = visit(returnNode->exprNode, ctx);
    this->should_return = true;
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_BreakNode(AstNode node, Context& ctx){
    this->should_break = true;
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_ContinueNode(AstNode node, Context& ctx){
    this->should_continue = true;
    return Null_sPtr;
}

Object_sPtr Interpreter::visit_ClassDefNode(AstNode node, Context& ctx) {
    std::shared_ptr<ClassDefNode> classDefNode = std::static_pointer_cast<ClassDefNode>(node);

    if (ctx.symbol_table->containsKeyAnywhere(classDefNode->name)) {
        throw Exception("Class '" + classDefNode->name + "' is already defined.");
    }

    std::shared_ptr<Class> newClass = std::shared_ptr<Class>(new Class(classDefNode->name));
    Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(newClass, true));
    ctx.symbol_table->addLocal(classDefNode->name, varWrapper);

    for (AstNode a : classDefNode->statements) {
        if (a->type == "VarDeclarationNode") {
            std::shared_ptr<VarDeclarationNode> varNode = std::static_pointer_cast<VarDeclarationNode>(a);
            newClass->addField(varNode->varName, Object_sPtr(new VariableWrapper(visit(varNode->exprNode, ctx), false)));
        } else if (a->type == "FunctionDefNode"){
            std::shared_ptr<FunctionDefNode> funDefNode = std::static_pointer_cast<FunctionDefNode>(a);
            Object_sPtr newFunction(new Function(funDefNode->name, funDefNode->argNames, funDefNode->statements));
            newClass->addField(funDefNode->name, Object_sPtr(new VariableWrapper(newFunction, false)));
        } else {
            throw Exception(a->type + " cannot be used in a class definition.");
        }
    }

    return newClass;
}

Object_sPtr Interpreter::visit_ConstructorCallNode(AstNode node, Context& ctx){
    std::shared_ptr<ConstructorCallNode> constructorCallNode = std::static_pointer_cast<ConstructorCallNode>(node);
    std::shared_ptr<FunctionCallNode> funCallNode = std::static_pointer_cast<FunctionCallNode>(constructorCallNode->funCallNode);

    Object_sPtr classDefinition = visit(funCallNode->nodeToCall, ctx);
    Object_sPtr newInstance = classDefinition->createInstance();

    Object_sPtr varWrapper = newInstance->getField("constructor");
    //varWrapper->constant_modifier = true;
    std::shared_ptr<Function> constructorObj = std::static_pointer_cast<Function>(varWrapper->getObject());

    constructorObj->isCallable();
    constructorObj->checkNumArgs(funCallNode->argNodes);

    Context funCtx = ctx.generateNewContext("Function "+ classDefinition->type +"'.constructor'");
    for (int i = 0; i < (int) constructorObj->argNames.size(); i++) {
        Object_sPtr arg_value = visit(funCallNode->argNodes.at(i), ctx);
        Object_sPtr varWrapper = Object_sPtr(new VariableWrapper(arg_value));
        funCtx.symbol_table->addLocal(constructorObj->argNames.at(i), varWrapper);
    }

    visit(AstNode(new VectorWrapperNode(constructorObj->statements)), funCtx);
    this->should_return = false;

    return newInstance;
}

Object_sPtr Interpreter::visit_AttributeAccessNode(AstNode node, Context& ctx){
    std::shared_ptr<AttributeAccessNode> attrAccessNode = std::static_pointer_cast<AttributeAccessNode>(node);
    Object_sPtr obj = visit(attrAccessNode->exprNode, ctx);

    Object_sPtr varWrapper = obj->getField(attrAccessNode->name);
    return varWrapper->getObject();
}

Object_sPtr Interpreter::visit_AttributeAssignNode(AstNode node, Context& ctx){
    std::shared_ptr<AttributeAssignNode> attrAssignNode = std::static_pointer_cast<AttributeAssignNode>(node);
    std::shared_ptr<AttributeAccessNode> attrAccessNode = std::static_pointer_cast<AttributeAccessNode>(attrAssignNode->attrNode);

    Object_sPtr obj = visit(attrAccessNode->exprNode, ctx);

    Object_sPtr varWrapper = obj->getField(attrAccessNode->name);
    Object_sPtr value = visit(attrAssignNode->exprNode, ctx);
    varWrapper->storeObject(value);

    return value;
}





