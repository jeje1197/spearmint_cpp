#ifndef ASTNODES_H_INCLUDED
#define ASTNODES_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Token.h"


class AstNodeBase {
    public:
        std::string type;

        virtual std::string toString() {
            return "Error: AstNode.toString()";
        }
};
typedef std::shared_ptr<AstNodeBase> AstNode;

class NumberNode : public AstNodeBase {
    public:
        std::string value;

        NumberNode(Token& tok) {
            this->type = "NumberNode";
            this->value = tok.value;
        }

        std::string toString() {
            return "(NumberNode: " + value + ")";
        }
};

class StringNode : public AstNodeBase {
    public:
        std::string value;

        StringNode(Token& tok) {
            this->type = "StringNode";
            this->value = tok.value;
        }

        std::string toString() {
            return "(StringNode: '" + value + "')";
        }
};

class UnaryOpNode : public AstNodeBase {
    public:
        std::string op;
        AstNode node = nullptr;

        UnaryOpNode(Token& opTok, AstNode node) {
            this->type = "UnaryOpNode";
            this->op = opTok.value;
            this->node = node;
        }

        std::string toString() {
            return "(UnaryOpNode Op: " + op + " Node: "+ node->toString() + ")";
        }
};

class BinOpNode : public AstNodeBase {
    public:
        AstNode left;
        std::string op;
        AstNode right;

        BinOpNode(AstNode left, Token& opTok, AstNode right) {
            this->type = "BinOpNode";
            this->left = left;
            this->op = opTok.value;
            this->right = right;
        }

        std::string toString() {
            return "(BinOpNode L: " + left->toString() + " Op: " + op + " R: " + right->toString() + ")";
        }
};

class VarDeclarationNode : public AstNodeBase {
    public:
        std::string varName;
        AstNode exprNode;

        VarDeclarationNode(Token& varNameTok, AstNode exprNode) {
            this->type = "VarDeclarationNode";
            this->varName = varNameTok.value;
            this->exprNode = exprNode;
        }

        std::string toString() {
            return "(VarDeclarationNode: " + varName + " = " + exprNode->toString() +")";
        }
};

class VarAssignNode : public AstNodeBase {
    public:
        std::string varName;
        AstNode exprNode;

        VarAssignNode(Token& varNameTok, AstNode exprNode) {
            this->type = "VarAssignNode";
            this->varName = varNameTok.value;
            this->exprNode = exprNode;
        }

        std::string toString() {
            return "(VarAssignNode: " + varName + " = " + exprNode->toString() + ")";
        }
};

class VarAccessNode : public AstNodeBase {
    public:
        std::string varName;

        VarAccessNode(Token& tok) {
            this->type = "VarAccessNode";
            this->varName = tok.value;
        }

        std::string toString() {
            return "(VarAccessNode: " + varName + ")";
        }
};

class IfNode : public AstNodeBase {
    public:
        std::vector<AstNode> caseConditions;
        std::vector<std::vector<AstNode>> caseStatements,
            elseCaseStatements;


        IfNode(std::vector<AstNode>& caseConditions, std::vector<std::vector<AstNode>>& caseStatements,
               std::vector<std::vector<AstNode>>& elseCaseStatements) {
            this->caseConditions = caseConditions;
            this->caseStatements = caseStatements;
            this->elseCaseStatements = elseCaseStatements;
        }

        std::string toString() {
            return "(IfNode: ...)";
        }

        std::string astListToString(std::vector<AstNode>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};

class ForNode : public AstNodeBase {
    public:
        AstNode initStatement, condNode, updateStatement;
        std::vector<AstNode> statements;

        ForNode(AstNode initStatement, AstNode condNode, AstNode updateStatement, std::vector<AstNode>& statements) {
            this->type = "ForNode";
            this->initStatement = initStatement;
            this->condNode = condNode;
            this->updateStatement = updateStatement;
            this->statements = statements;
        }

        std::string toString() {
            return "(ForNode I: " + initStatement->toString() + " C: " + condNode->toString() + " U: " +
                updateStatement->toString() + " do {" + astListToString(statements) + "})";
        }

        std::string astListToString(std::vector<AstNode>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};

class WhileNode : public AstNodeBase {
    public:
        AstNode condNode;
        std::vector<AstNode> statements;

        WhileNode(AstNode condNode, std::vector<AstNode>& statements) {
            this->type = "WhileNode";
            this->condNode = condNode;
            this->statements = statements;
        }

        std::string toString() {
            return "(WhileNode Cond:" + condNode->toString() + " do {" + astListToString(statements) + "})";
        }

        std::string astListToString(std::vector<AstNode>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};

class FunctionDefNode : public AstNodeBase {
    public:
        std::string name;
        std::vector<std::string> argNames;
        std::vector<AstNode> statements;

        FunctionDefNode(Token& functionNameToken, std::vector<std::string>& argNames, std::vector<AstNode>& statements) {
            this->type = "FunctionNode";
            this->name = functionNameToken.value;
            this->argNames = argNames;
            this->statements = statements;
        }

        std::string toString() {
            return "(FunctionNode Name: '" + name + + "' Args:(" + stringListToString(argNames) +
                ") do {" + astListToString(statements) + "})";
        }

        std::string stringListToString(std::vector<std::string>& list) {
            std::string output;

            for (int i = 0; i < (int) list.size(); i++) {
                output += list.at(i) + ", ";
            }

            return output;
        }

        std::string astListToString(std::vector<AstNode>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};

class FunctionCallNode : public AstNodeBase {
    public:
        std::string name;
        std::vector<AstNode> argNodes;

        FunctionCallNode(Token& functionNameToken, std::vector<AstNode>& argNodes) {
            this->type = "FunctionCallNode";
            this->name = functionNameToken.value;
            this->argNodes = argNodes;
        }

        std::string toString() {
            return "(FunctionCallNode Name: '" + name + + "' Args:(" +
                astListToString(argNodes) + ")";
        }

        std::string astListToString(std::vector<AstNode>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};




#endif // ASTNODES_H_INCLUDED
