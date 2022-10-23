#ifndef BUILTINFUNCTIONS_H_INCLUDED
#define BUILTINFUNCTIONS_H_INCLUDED

#include "Classes.h"
#include "Context.h"

Object_sPtr print(Context* ctx) {
    Object_sPtr varWrapper = ctx->symbol_table->get("text");
    Object_sPtr strObj = varWrapper->getObject();

    std::cout << strObj->toString() << std::endl;
    return Object::NullType();
}

Object_sPtr printFunction(new Function("print", {"text"}, (Object_sPtr (*)(void *))&print));

#endif // BUILTINFUNCTIONS_H_INCLUDED
