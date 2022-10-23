#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <cmath>

class Object;
typedef std::shared_ptr<Object> Object_sPtr;

class Object {
    // Base Object in Spearmint
    public:
        std::string type = "BaseObject"; // This is for type management
        std::string str_value = "Base Object String"; // This is for strings
        //std::string name = "BaseObject"; // This is for functions
        float float_value; // This is for numbers
        bool boolean_value = false; // This is for booleans

        Object() {}

        Object(std::string type) {
            this->type = type;
        }

        virtual ~Object() = default;

        // Base Methods for all types
        std::string getType() {
            return type;
        }

        std::string getStrValue() {
            return str_value;
        }

        float getFloatValue() {
            return float_value;
        }

        bool getBooleanValue() {
            return boolean_value;
        }

        int getIntValue() {
            return (int) float_value;
        }

        virtual bool isConstant() {
            illegalOperation();
            return false;
        }

        virtual void storeObject(Object_sPtr obj) {
            illegalOperation();
        }

        virtual Object_sPtr getObject() {
            return illegalOperation();
        }

        bool isCallable() {
            if (type != "Function") {
                throw Exception(type + " is not callable");
            }
            return false;
        }

        virtual bool checkNumArgs(std::vector<AstNode>& other) {
            illegalOperation();
            return false;
        }

        std::string getAddress() {
            const void * address = static_cast<const void*>(this);
            std::stringstream ss;
            ss << address;
            return ss.str();
        }

        bool isInstance(Object_sPtr obj, std::string type) {
            return obj->type.compare(type) == 0;
        }

        bool isNull() {
            return type.compare("Null") == 0;
        }

        virtual std::string toString() {
            if (this->isNull()) {
                return "null";
            }
            return type + getAddress();
        }

        static Object_sPtr NullType() {
            return Object_sPtr(new Object("Null"));
        }

        // Operator methods
        Object_sPtr illegalOperation() {
            throw Exception("Operation cannot be performed on " + this->type);
            return NullType();
        }

        virtual Object_sPtr add(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr sub(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr mul(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr div(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr pow(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr mod(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_lt(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_gt(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_lte(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_gte(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_ee(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr compare_ne(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr anded_by(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr ored_by(Object_sPtr other) {
            return illegalOperation();
        }

        virtual Object_sPtr notted() {
            return illegalOperation();
        }

        virtual bool is_true() {
            return !isNull();
        }

        virtual int getSizeInternal() {
            illegalOperation();
            return 0;
        }

        virtual Object_sPtr getInternal(int index) {
            return illegalOperation();
        }
};

class VariableWrapper : public Object {
    private:
        Object_sPtr obj;
        bool constant_modifier = false; // 0 - none, 1 - const

    public:
        VariableWrapper(Object_sPtr obj) : Object("VariableWrapper") {
            this->obj = obj;
        }

        VariableWrapper(Object_sPtr obj, bool isConstant) : Object("VariableWrapper") {
            this->obj = obj;
            this->constant_modifier = isConstant;
        }

        void storeObject(Object_sPtr obj) {
            this->obj = obj;
        }

        Object_sPtr getObject() {
            return obj;
        }

        bool isConstant() {
            return constant_modifier;
        }
};

class Boolean : public Object {
    public:
        Boolean(bool value) : Object("Boolean") {
            this->boolean_value = value;
        }

        Boolean(std::string value) : Object("Boolean") {
            this->boolean_value = value.compare("") != 0;
        }

        Boolean(float value) : Object("Boolean") {
            this->boolean_value = value != 0;
        }

        std::string toString() {
            return this->boolean_value ? "true" : "false";
        }

        bool is_true() {
            return this->boolean_value;
        }

        Object_sPtr anded_by(Object_sPtr other) {
            return Object_sPtr(new Boolean(is_true() && other->is_true()));
        }

        Object_sPtr ored_by(Object_sPtr other) {
            return Object_sPtr(new Boolean(is_true() || other->is_true()));
        }

        Object_sPtr notted() {
            return Object_sPtr(new Boolean(!is_true()));
        }

};

class String : public Object {
    public:
        String(std::string value) : Object("String") {
            this->str_value = value;
        }

        std::string toString() {
            return "'" + str_value + "'";
        }

        // Operations
        Object_sPtr add(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new String(this->getStrValue() + other->getStrValue()));
            }
            return Object_sPtr(new String(this->getStrValue() + other->toString()));
        }

        Object_sPtr compare_lt(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) < 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gt(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) > 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_lte(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) <= 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gte(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) >= 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ee(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) == 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ne(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Boolean(this->getStrValue().compare(other->getStrValue()) != 0));
            }
            return illegalOperation();
        }

        Object_sPtr notted() {
            return Object_sPtr(new Boolean(this->getStrValue().compare("") == 0));
        }

        bool is_true() {
            return this->getStrValue().compare("") != 0;
        }
};

class Number : public Object {
    public:
        Number(float value) : Object("Number") {
            this->float_value = value;
        }

        std::string toString() {
            return std::to_string(this->float_value);
        }

        Object_sPtr add(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(this->getFloatValue() + other->getFloatValue()));
            } else if (isInstance(other, "String"))  {
                return Object_sPtr(new String(this->toString() + other->getStrValue()));
            }
            return illegalOperation();
        }

        Object_sPtr sub(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(this->getFloatValue() - other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr mul(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(this->getFloatValue() * other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr div(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(this->getFloatValue() / other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr pow(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(std::pow(this->getFloatValue(), other->getIntValue())));
            }
            return illegalOperation();
        }

        Object_sPtr mod(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Number(std::fmod(this->getFloatValue(), other->getFloatValue())));
            }
            return illegalOperation();
        }

        Object_sPtr compare_lt(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() < other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gt(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() > other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_lte(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() <= other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gte(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() >= other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ee(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() == other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ne(Object_sPtr other) {
            if (isInstance(other, "Number"))  {
                return Object_sPtr(new Boolean(this->getFloatValue() != other->getFloatValue()));
            }
            return illegalOperation();
        }

        Object_sPtr notted() {
            return Object_sPtr(new Number(!is_true()));
        }

        bool is_true() {
            return this->getFloatValue() != 0;
        }

};

class List : public Object {
    private:
        std::vector<Object_sPtr> myList;

    public:
        List() : Object("List") {}

        int getSizeInternal() {
            return (int) this->myList.size();
        }

        Object_sPtr getInternal(int index) {
            return this->myList.at(index);
        }

        Object_sPtr add(Object_sPtr newObj) {
            this->myList.push_back(newObj);
            return Object::NullType();
        }

        Object_sPtr get(Object_sPtr numObj) {
            if (!isInstance(numObj, "Number")){
                throw Exception("List get method requires a number object as argument.");
            }
            int index = numObj->getIntValue();
            if ((index < 0) || (index >= (int) myList.size())) {
                throw Exception("Index " + std::to_string(index) + " is out of list bounds.");
            }
            this->myList.at(index);
            return Object::NullType();
        }

        Object_sPtr getSize() {
            return Object_sPtr(new Number(myList.size()));
        }

        std::string toString() {
            std::string str = "[";
            for (int i = 0; i < (int) myList.size(); i++) {
                str += myList.at(i)->toString();
                if (i < (int) myList.size() - 1) {
                    str += ", ";
                }
            }
            str += "]";
            return str;
        }

        bool is_true() {
            return !myList.empty();
        }

};

class Function : public Object {
    public:
        std::string name;
        std::vector<std::string> argNames;
        std::vector<AstNode> statements;
        bool builtIn = false;
        Object_sPtr (*execute)(void*) = nullptr;

        Function(std::string name, std::vector<std::string> argNames, std::vector<AstNode> statements) : Object("Function") {
            this->name = name;
            this->argNames = argNames;
            this->statements = statements;
        }

        Function(std::string name, std::vector<std::string> argNames, Object_sPtr (*execute)(void*)) : Object("Function") {
            this->name = name;
            this->argNames = argNames;
            this->execute = execute;
            this->builtIn = true;
        }

        bool checkNumArgs(std::vector<AstNode> other) {
            int numArgs = (int) argNames.size();
            int numPassedArgs = (int) other.size();

            if (numArgs != numPassedArgs) {
                throw Exception("Function '" + name + "' expected " + std::to_string(numArgs) + " args, but received " +
                                std::to_string(numPassedArgs) + " args");
            }
            return true;
        }

        bool isBuiltIn() {
            return builtIn;
        }

        Object_sPtr executeWrapper(void* ctx) {
            if (execute == nullptr) {
                throw Exception("Built in method not defined for " + name);
            }

            // Should call function from function pointer (void (*)(void *))
            return execute(ctx);
        }

        std::string toString() {
            return "Function '" + name + "' (" + std::to_string((int) argNames.size()) + ") at address:" + getAddress();
        }

};

#endif // CLASSES_H_INCLUDED
