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

        IfNode(Token& tok) {
            //this->type = "IfNode";
            //this->varName = tok.value;
        }

        std::string toString() {
            return "(IfNode:)";
        }
};

class ForNode : public AstNodeBase {
    public:
        AstNode initNode, condNode, updateNode;
        std::vector<AstNode> statements;

        ForNode(AstNode initNode, AstNode condNode, AstNode updateNode, std::vector<AstNode>& statements) {
            this->type = "ForNode";
            this->initNode = initNode;
            this->condNode = condNode;
            this->updateNode = updateNode;
            this->statements = statements;
        }

        std::string toString() {
            return "(ForNode I: " + initNode->toString() + " C: " + condNode->toString() + " U: " +
                updateNode->toString() + " do {})";
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

            for (int i=0; i < ast.size(); i++) {
                output += ast.at(i)->toString();
            }

            return output;
        }
};






#endif // ASTNODES_H_INCLUDED
