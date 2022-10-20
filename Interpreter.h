#pragma once
#include "Exception.h"
#include "AstNodes.h"
#include "Classes.h"
#include <string>
#include <vector>

class Interpreter {
	public:
		std::string fileName;
		Object_sPtr result;

		bool should_return = false;
		bool should_break = false;
		bool should_continue = false;

		Interpreter();
		Interpreter(std::string fileName);
		Object_sPtr visit(AstNode vector_or_node);
        Object_sPtr visit_VectorWrapperNode(AstNode node);

		Object_sPtr visit_NumberNode(AstNode node);
		Object_sPtr visit_StringNode(AstNode node);
		Object_sPtr visit_UnaryOpNode(AstNode node);
		Object_sPtr visit_BinOpNode(AstNode node);
		Object_sPtr visit_VarDeclarationNode(AstNode node);
		Object_sPtr visit_VarAssignNode(AstNode node);
		Object_sPtr visit_VarAccessNode(AstNode node);
};
