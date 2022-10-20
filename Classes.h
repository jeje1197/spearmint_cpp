#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>
#include <sstream>

class Object;
class Number;
class String;
class List;
typedef std::shared_ptr<Object> Object_sPtr;

class Object {
    // Base Object in Spearmint
    public:
        std::string type = "BaseObject";
        std::string str_value = "Base Object String";
        float float_value;

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

        int getIntValue() {
            return (int) float_value;
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
            }
            return illegalOperation();
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
                return Object_sPtr(new String(this->getStrValue()+ other->getStrValue()));
            }
            return illegalOperation();
        }

        Object_sPtr compare_lt(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) < 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gt(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) > 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_lte(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) <= 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_gte(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) >= 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ee(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) == 0));
            }
            return illegalOperation();
        }

        Object_sPtr compare_ne(Object_sPtr other) {
            if (isInstance(other, "String"))  {
                return Object_sPtr(new Number(this->getStrValue().compare(other->getStrValue()) != 0));
            }
            return illegalOperation();
        }

        Object_sPtr notted() {
            return Object_sPtr(new Number((float) this->getStrValue().compare("") == 0));
        }

        bool is_true() {
            return this->getStrValue().compare("") != 0;
        }
};

class List : public Object {
    private:
        std::vector<Object_sPtr> myList;

    public:
        List() : Object("List") {}

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



};


#endif // CLASSES_H_INCLUDED