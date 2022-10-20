#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "Exception.h"
#include "Classes.h"
#include <string>
#include <unordered_map>

class SymbolTable {
    std::unordered_map<std::string, Object_sPtr> symbol_table;
    SymbolTable* parent = nullptr;

    public:
        SymbolTable() {}

        SymbolTable(SymbolTable* parent) {
            this->parent = parent;
        }

        bool containsLocalKey(std::string key) {
            return this->symbol_table.find(key) != this->symbol_table.end();
        }

        bool containsKeyAnywhere(std::string key) {
            SymbolTable* cur = this;
            while (cur != nullptr) {
                if (cur->containsLocalKey(key)) {
                    return true;
                }
                cur = cur->parent;
            }
            return false;
        }

        void addLocal(std::string key, Object_sPtr value) {
            this->symbol_table[key] = value;
        }

        void update(std::string key, Object_sPtr value) {
            SymbolTable* cur = this;
            while (cur != nullptr) {
                if (cur->containsLocalKey(key)) {
                    cur->symbol_table[key] = value;
                    return;
                }
                cur = cur->parent;
            }
        }

        Object_sPtr get(std::string key) {
            SymbolTable* cur = this;
            while (cur != nullptr) {
                if (cur->containsLocalKey(key)) {
                    return cur->symbol_table.at(key);
                }
                cur = cur->parent;
            }
            throw Exception("'" + key + "' is not in scope.");
            return Object::NullType();
        }

        void remove(std::string key) {
            this->symbol_table.erase(key);
        }
};

class Context {
    public:
        std::string name;
        SymbolTable* symbol_table;

        Context(std::string name, SymbolTable symbol_table) {
            this->name = name;
            this->symbol_table = &symbol_table;
        }

        Context generateNewContext(std::string name) {
            Context c = Context(name, SymbolTable(symbol_table));
            return c;
        }

};


#endif // CONTEXT_H_INCLUDED
