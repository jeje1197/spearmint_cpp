#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include <string>
#include <iostream>
class Exception {
    public:
        std::string message;

        Exception(std::string message);
        Exception(std::string message, char charToAppend);

        std::string getMessage();
        void show();
};

#endif // EXCEPTION_H_INCLUDED
