#ifndef BUILTINFUNCTIONS_H_INCLUDED
#define BUILTINFUNCTIONS_H_INCLUDED

#include "Classes.h"
#include "Context.h"

Object_sPtr print(Context& ctx) {
    Object_sPtr varWrapper = ctx.symbol_table->get("text");
    std::cout << varWrapper->getObject()->toString() << std::endl;
    return Object::NullType();
}

std::vector<std::string> print_args = {"text"};
Object_sPtr printFunction(new Function("print", {"text"}, (void (*)(void *))&print));

#endif // BUILTINFUNCTIONS_H_INCLUDED
