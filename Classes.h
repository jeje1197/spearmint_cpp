#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

class Object {
    // Base Object in Spearmint
    std::string type;
    public:
        Object() {
            this->type = "BaseObject";
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

class VectorWrapper : public Object {
    std::vector<AstNode> v;

    public:
      VectorWrapper(std::vector<AstNode> v) : Object("VectorWrapper") {
          this->v = v;
      }
};


class Number : public Object {

};

class String : public Object {
};



#endif // CLASSES_H_INCLUDED
