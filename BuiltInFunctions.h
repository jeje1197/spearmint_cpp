#ifndef BUILTINFUNCTIONS_H_INCLUDED
#define BUILTINFUNCTIONS_H_INCLUDED

#include "Classes.h"
#include "Context.h"

typedef std::shared_ptr<Function> Function_sPtr;

// Print function
Object_sPtr print(Context* ctx) {
    Object_sPtr varWrapper = ctx->symbol_table->get("text");
    Object_sPtr strObj = varWrapper->getObject();

    std::cout << strObj->toString() << std::endl;
    return Object::NullType();
}
Function_sPtr printFunction(new Function("print", {"text"}, (Object_sPtr (*)(void *))&print));

// Get type
Object_sPtr type(Context* ctx) {
    Object_sPtr varWrapper = ctx->symbol_table->get("object");
    Object_sPtr obj = varWrapper->getObject();

    return Object_sPtr(new String(varWrapper->getObject()->getType()));
}
Function_sPtr typeFunction(new Function("type", {"object"}, (Object_sPtr (*)(void *))&type));

// Get type
Object_sPtr isNull(Context* ctx) {
    Object_sPtr varWrapper = ctx->symbol_table->get("object");
    Object_sPtr obj = varWrapper->getObject();

    return Object_sPtr(new Boolean(varWrapper->getObject()->getType() == "Null"));
}
Function_sPtr isNullFunction(new Function("isNull", {"object"}, (Object_sPtr (*)(void *))&isNull));

// Get type
Object_sPtr len(Context* ctx) {
    Object_sPtr varWrapper = ctx->symbol_table->get("object");
    Object_sPtr obj = varWrapper->getObject();

    return Object_sPtr(new Number(varWrapper->getObject()->getLength()));
}
Function_sPtr lenFunction(new Function("len", {"object"}, (Object_sPtr (*)(void *))&len));

// Read console input
Object_sPtr input(Context* ctx) {
    std::string input;
    getline(std::cin, input);
    return Object_sPtr(new String(input));
}
Function_sPtr inputFunction(new Function("input", {}, (Object_sPtr (*)(void *))&input));

// Exit program function
Object_sPtr closeProgram(Context* ctx) {
    exit(0);
    return Object::NullType();
}
Function_sPtr exitFunction(new Function("exit", {}, (Object_sPtr (*)(void *))&closeProgram));

// Function to add all built-in functions to SymbolTable
std::vector<Function_sPtr> BUILTINFUNCTIONS = {printFunction, typeFunction, isNullFunction, lenFunction, inputFunction,
    exitFunction};

void addBuiltInFunctions(SymbolTable_sPtr symbol_table) {
    for (Function_sPtr funPtr: BUILTINFUNCTIONS) {
        symbol_table->addLocal(funPtr->name, Object_sPtr(new VariableWrapper(funPtr, true)));
    }
}
#endif // BUILTINFUNCTIONS_H_INCLUDED
