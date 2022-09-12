#ifndef ASTNODES_H_INCLUDED
#define ASTNODES_H_INCLUDED

#include <iostream>
#include <memory>
#include "Token.h"

using std::string;
using std::shared_ptr;

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
        shared_ptr<AstNode> node = nullptr;

        UnaryOpNode(Token opTok, shared_ptr<AstNode> node) {
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
        shared_ptr<AstNode> left;
        string op;
        shared_ptr<AstNode> right;

        BinOpNode(shared_ptr<AstNode> left, Token opTok, shared_ptr<AstNode> right) {
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
