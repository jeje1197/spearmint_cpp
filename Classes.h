#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>
#include <sstream>

class Object;
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
            return this->type;
        }

        std::string getStrValue() {
            return this->str_value;
        }

        float getFloatValue() {
            return this->float_value;
        }

        int getIntValue() {
            return (int) this->float_value;
        }

        std::string getAddress() {
            const void * address = static_cast<const void*>(this);
            std::stringstream ss;
            ss << address;
            return ss.str();
        }

        bool isInstance(Object_sPtr obj, std::string type) {
            return obj->type == type;
        }

        bool isSameType(Object& obj1, Object& obj2) {
            return (obj1.type).compare(obj2.type) == 0;
        }

        bool isNull() {
            return this->type == "Null";
        }

        virtual std::string toString() {
            if (this->isNull()) {
                return "null";
            }
            return this->type + this->getAddress();
        }

        static Object_sPtr NullType() {
            return Object_sPtr(new Object("Null"));
        }

        // Operator methods
        void illegalOperation() {
            throw Exception("Operation cannot be performed on " + this->type);
        }

        virtual Object_sPtr add(Object_sPtr other) {
            std::cout << "Base object called" << std::endl;
            illegalOperation();
            return NullType();
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
            if (other->getType() == "Number")  {
                return Object_sPtr(new Number(this->getFloatValue() + other->getFloatValue()));
            } else {
                this->illegalOperation();
            }
            return NullType();
        }
};

class String : public Object {
    public:
        String(std::string value) {
            this->str_value = value;
        }

        std::string toString() {
            return this->str_value;
        }

        Object_sPtr add(Object_sPtr other) {
            if (other->getType() == "String")  {
                return Object_sPtr(new String(this->getStrValue()+ other->getStrValue()));
            } else {
                this->illegalOperation();
            }
            return NullType();
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
            if ((index < 0) || (index >= this->myList.size())) {
                throw Exception("Index " + std::to_string(index) + " is out of list bounds.");
            }
            this->myList.at(index);
            return Object::NullType();
        }

        Object_sPtr getSize() {
            return Object_sPtr(new Number(this->myList.size()));
        }

        std::string toString() {
            std::string str = "[";
            for (int i = 0; i < this->myList.size(); i++) {
                str += this->myList.at(i)->toString();
                if (i < this->myList.size() - 1) {
                    str += ", ";
                }
            }
            str += "]";
            return str;
        }



};


#endif // CLASSES_H_INCLUDED
