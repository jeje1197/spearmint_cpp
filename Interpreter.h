#pragma once
#include "Exception.h"
#include "AstNodes.h"
#include "Classes.h"
#include "Context.h"
#include <string>
#include <vector>

class Interpreter {
	public:
		std::string fileName;

		Object_sPtr return_value = nullptr;
		bool should_return = false;
		bool should_break = false;
		bool should_continue = false;

		Interpreter();
		Interpreter(std::string fileName);
		Object_sPtr visit(AstNode vector_or_node, Context& ctx);
        Object_sPtr visit_VectorWrapperNode(AstNode node, Context& ctx);

		Object_sPtr visit_NumberNode(AstNode node, Context& ctx);
		Object_sPtr visit_StringNode(AstNode node, Context& ctx);
		Object_sPtr visit_UnaryOpNode(AstNode node, Context& ctx);
		Object_sPtr visit_BinOpNode(AstNode node, Context& ctx);
		Object_sPtr visit_VarDeclarationNode(AstNode node, Context& ctx);
		Object_sPtr visit_VarAssignNode(AstNode node, Context& ctx);
		Object_sPtr visit_VarAccessNode(AstNode node, Context& ctx);
		Object_sPtr visit_IfNode(AstNode node, Context& ctx);
		Object_sPtr visit_ForNode(AstNode node, Context& ctx);
		Object_sPtr visit_WhileNode(AstNode node, Context& ctx);

		Object_sPtr visit_FunctionDefNode(AstNode node, Context& ctx);
		Object_sPtr visit_FunctionCallNode(AstNode node, Context& ctx);

		Object_sPtr visit_ReturnNode(AstNode node, Context& ctx);
		Object_sPtr visit_BreakNode(AstNode node, Context& ctx);
		Object_sPtr visit_ContinueNode(AstNode node, Context& ctx);

		Object_sPtr visit_ClassDefNode(AstNode node, Context& ctx);
		Object_sPtr visit_ConstructorCallNode(AstNode node, Context& ctx);
		Object_sPtr visit_AttributeAccessNode(AstNode node, Context& ctx);
		Object_sPtr visit_AttributeAssignNode(AstNode node, Context& ctx);

		Object_sPtr visit_ListNode(AstNode node, Context& ctx);
};
