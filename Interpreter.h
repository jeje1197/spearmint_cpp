#pragma once
#include "Exception.h"
#include "AstNodes.h"
#include "Classes.h"
#include <string>

class Interpreter {
	public:
		std::string fileName;
		Interpreter();
		Interpreter(std::string fileName);
		Object visit(AstNode vector_or_node);
        Object visit_VectorWrapperNode(AstNode node);

		Number visit_NumberNode(AstNode node);
		String visit_StringNode(AstNode node);
		Object visit_UnaryOpNode(AstNode node);
		Object visit_BinOpNode(AstNode node);
};
