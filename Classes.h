#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>

class Object;
typedef std::unique_ptr<Object> Object_uPtr;

class Object {
    // Base Object in Spearmint
    public:
        std::string type;
        std::string str_value;
        float float_value;

        Object() {
            this->type = "BaseObject";
            this->str_value = "BaseObjectValue";
        }

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

        bool isSameType(Object& obj1, Object& obj2) {
            return (obj1.type).compare(obj2.type) == 0;
        }

        virtual std::string toString() {
            return this->type + " at address <>";
        }

        // Operator methods
        void illegalOperation() {
            throw Exception("Operation cannot be performed on " + this->type);
        }

        virtual Object_uPtr add(Object other) {
            std::cout << "Base object called";
            illegalOperation();
        }
};

class NullType : public Object {
    public:
        NullType() : Object("Null") {
        }
};

class Number : public Object {
    public:
        Number(float value) : Object() {
            this->type = "Number";
            this->float_value = value;
        }

        std::string toString() {
            return std::to_string(this->float_value);
        }

        Object_uPtr add(Object_uPtr other) {
            std::cout << other->getType();
            if (other->getType() == "Number")  {
                return Object_uPtr(new Number(this->getFloatValue() + other->getFloatValue()));
            } else {
                this->illegalOperation();
            }
        }
};

class String : public Object {
    public:
        String(std::string value) {
            this->type = "String";
            this->str_value = value;
        }

        std::string toString() {
            return this->str_value;
        }

        Object_uPtr add(Object_uPtr other) {
            if (other->getType() == "String")  {
                return Object_uPtr(new String(this->str_value + other->str_value));
            } else {
                this->illegalOperation();
            }
        }

};


#endif // CLASSES_H_INCLUDED
