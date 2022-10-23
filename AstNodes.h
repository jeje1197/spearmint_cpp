#ifndef ASTNODES_H_INCLUDED
#define ASTNODES_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Token.h"

class AstNodeBase;
typedef std::shared_ptr<AstNodeBase> AstNode;

class AstNodeBase {
    public:
        std::string type;
        Position startPos;

        virtual std::string toString() {
            return "Error: AstNode.toString()";
        }

        virtual ~AstNodeBase() = default;

        static std::string stringListToString(std::vector<std::string>& list) {
            std::string output;

            for (int i = 0; i < (int) list.size(); i++) {
                output += list.at(i) + ", ";
            }

            return output;
        }

        static std::string astListToString(std::vector<std::shared_ptr<AstNodeBase>>& ast) {
            std::string output;

            for (int i = 0; i < (int) ast.size(); i++) {
                output += ast.at(i)->toString() + " ";
            }

            return output;
        }
};

// This purpose of this is to be able to pass a vector to the
// visit method of the Interpreter
class VectorWrapperNode : public AstNodeBase {
    std::vector<AstNode> v;
    public:
        VectorWrapperNode(std::vector<AstNode> v) {
            this->type = "VectorWrapperNode";
            this->v = v;
        }

        std::vector<AstNode> getVector() {
            return this->v;
        }
};

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
        AstNode exprNode;

        UnaryOpNode(Token& opTok, AstNode exprNode) {
            this->type = "UnaryOpNode";
            this->op = opTok.value;
            this->exprNode = exprNode;
        }

        std::string toString() {
            return "(UnaryOpNode Op: " + op + " Node: "+ exprNode->toString() + ")";
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
        bool isConstant;

        VarDeclarationNode(Token& varNameTok, AstNode exprNode, bool isConstant) {
            this->type = "VarDeclarationNode";
            this->varName = varNameTok.value;
            this->exprNode = exprNode;
            this->isConstant = isConstant;
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
        std::vector<std::vector<AstNode>> caseStatements;
        std::vector<AstNode> elseCaseStatements;


        IfNode(std::vector<AstNode>& caseConditions, std::vector<std::vector<AstNode>>& caseStatements,
               std::vector<AstNode>& elseCaseStatements) {
            this->type = "IfNode";
            this->caseConditions = caseConditions;
            this->caseStatements = caseStatements;
            this->elseCaseStatements = elseCaseStatements;
        }

        std::string toString() {
            return "(IfNode: cases...)";
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
};

class FunctionDefNode : public AstNodeBase {
    public:
        std::string name;
        std::vector<std::string> argNames;
        std::vector<AstNode> statements;

        FunctionDefNode(Token& functionNameTok, std::vector<std::string>& argNames, std::vector<AstNode>& statements) {
            this->type = "FunctionDefNode";
            this->name = functionNameTok.value;
            this->argNames = argNames;
            this->statements = statements;
        }

        std::string toString() {
            return "(FunctionDefNode Name: '" + name + + "' Args:(" + stringListToString(argNames) +
                ") do {" + astListToString(statements) + "})";
        }
};

class FunctionCallNode : public AstNodeBase {
    public:
        AstNode nodeToCall;
        std::vector<AstNode> argNodes;

        FunctionCallNode(AstNode nodeToCall, std::vector<AstNode>& argNodes) {
            this->type = "FunctionCallNode";
            this->nodeToCall = nodeToCall;
            this->argNodes = argNodes;
        }

        std::string toString() {
            return "(FunctionCallNode NodeToCall: " + nodeToCall->toString() + ", Args: " +
                astListToString(argNodes) + ")";
        }
};



class ClassDefNode : public AstNodeBase {
    public:
        std::string name;
        std::vector<AstNode> statements;

        ClassDefNode(Token& classNameTok, std::vector<AstNode>& statements) {
            this->type = "FunctionCallNode";
            this->name = classNameTok.value;
            this->statements = statements;
        }

        std::string toString() {
            return "(ClassDefNode Name: '" + name + + "' Statements: {" +
                astListToString(statements) + "})";
        }
};




#endif // ASTNODES_H_INCLUDED
