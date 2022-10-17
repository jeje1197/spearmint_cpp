#pragma once
#include "AstNodes.h"
#include "Classes.h"
#include <string>

class Interpreter {
	public:
		std::string fileName;
		Interpreter();
		Interpreter(std::string fileName);
		Object visit(AstNode vector_or_node);
		Number visit_NumberNode(AstNode node);
};
