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
		Object_uPtr visit(AstNode vector_or_node);
        Object_uPtr visit_VectorWrapperNode(AstNode node);

		Object_uPtr visit_NumberNode(AstNode node);
		Object_uPtr visit_StringNode(AstNode node);
		Object_uPtr visit_UnaryOpNode(AstNode node);
		Object_uPtr visit_BinOpNode(AstNode node);
};
