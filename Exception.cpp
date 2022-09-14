#include "Exception.h"

Exception::Exception(std::string message) {
    this->message = message;
}

Exception::Exception(std::string message, char charToAppend) {
    message.push_back(charToAppend);
    this->message = message;
}

std::string Exception::getMessage() {
    return this->message;
}

void Exception::show() {
    std::cout << this->getMessage() << std::endl;
}

