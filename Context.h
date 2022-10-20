#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "Exception.h"
#include "Classes.h"
#include <unordered_map>

class SymbolTable {
    unordered_map<std::string, Object_sPtr> symbol_table;
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
                if (cur.containsLocalKey(key)) {
                    return true;
                }
                cur = cur->parent;
            }
            return false;
        }

        void addLocal(std::string key, Object_sPtr value) {
            if (this->containsLocalKey(key)) {
                throw Exception("'" + key + "' is already defined in scope.");
            }
            this->symbol_table.insert(key, value);
        }

        void update(std::string key, Object_sPtr value) {
            SymbolTable* cur = this;
            while (cur != nullptr) {
                if (cur->containsLocalKey(key)) {
                    return cur->symbol_table.insert(key, value);
                }
                cur = cur->parent;
            }
            throw Exception("Cannot update key, value pair. '" + key + "' has not been declared.");
        }

        Object_sPtr get(std::string key) {
            SymbolTable* cur = this;
            while (cur != nullptr) {
                if (cur->containsLocalKey(key)) {
                    return cur->symbol_table.at(key);
                }
                cur = cur->parent;
            }
            throw Exception("'" + key "' is not in scope.");
            return Null_sPtr;
        }

        void remove(std::string key) {
            this->symbol_table.erase(key);
        }
};

class Context {

    public:
        Context() {
        }
};


#endif // CONTEXT_H_INCLUDED
