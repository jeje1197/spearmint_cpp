#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

class Object {
    // Base Object in Spearmint
    std::string type;
    std::string value;
    public:
        Object() {
            this->type = "BaseObject";
            this->value = "BaseObjectValue";
        }

        Object(std::string type) {
            this->type = type;
        }

        virtual ~Object() = default;

        std::string getType() {
            return this->type;
        }

        virtual std::string toString() {
            return "Base Object at address <>";
        }

};

class NullType : public Object {
    public:
        NullType() : Object("Null") {
        }
};


class Number : public Object {
    std::string value;
    public:
        Number(std::string value) {
            this->type = "Number";
            this->value = value;
        }
};

class String : public Object {
};



#endif // CLASSES_H_INCLUDED
