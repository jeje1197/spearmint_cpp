#ifndef ASTNODES_H_INCLUDED
#define ASTNODES_H_INCLUDED

#include <iostream>
#include <memory>
#include "Token.h"

class AstNode {
    public:
        std::string type;

        virtual std::string toString() {
            return "Error: AstNode.toString()";
        }
};

AstNode NONE;

class NumberNode : public AstNode {
    public:
        std::string value;

        NumberNode(Token tok) {
            this->type = "NumberNode";
            this->value = tok.value;
        }

        std::string toString() {
            return "(NumberNode: " + value + ")";
        }
};

class StringNode : public AstNode {
    public:
        std::string value;

        StringNode(Token tok) {
            this->type = "StringNode";
            this->value = tok.value;
        }

        std::string toString() {
            return "(StringNode: '" + value + "')";
        }
};

class UnaryOpNode : public AstNode {
    public:
        std::string op;
        AstNode* node;

        UnaryOpNode(Token opTok, AstNode* node) {
            this->type = "UnaryOpNode";
            this->op = opTok.value;
            this->node = node;
        }

        std::string toString() {
            return "(UnaryOpNode Op: " + op + " Node: "+ node->toString() + ")";
        }
};

class BinOpNode : public AstNode {
    public:
        AstNode* left;
        std::string op;
        AstNode* right;

        BinOpNode(AstNode* left, Token opTok, AstNode* right) {
            this->type = "BinOpNode";
            this->left = left;
            this->op = opTok.value;
            this->right = right;
        }

        std::string toString() {
            return "(BinOpNode L: " + left->toString() + " Op: " + op + " R: " + right->toString() + ")";
        }
};

#endif // ASTNODES_H_INCLUDED
