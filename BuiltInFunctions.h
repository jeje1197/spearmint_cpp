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

// Exit program function
Object_sPtr closeProgram(Context* ctx) {
    exit(0);
    return Object::NullType();
}
Function_sPtr exitFunction(new Function("exit", {}, (Object_sPtr (*)(void *))&closeProgram));



// Function to add all built-in functions to SymbolTable
std::vector<Function_sPtr> BUILTINFUNCTIONS = {printFunction, exitFunction};
void addBuiltInFunctions(SymbolTable_sPtr symbol_table) {
    for (Function_sPtr funPtr: BUILTINFUNCTIONS) {
        symbol_table->addLocal(funPtr->name, Object_sPtr(new VariableWrapper(funPtr, true)));
    }
}
#endif // BUILTINFUNCTIONS_H_INCLUDED
