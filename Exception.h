#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include <string>
class Exception {
    public:
        std::string message;

        Exception(std::string message);

        std::string getMessage();
};

#endif // EXCEPTION_H_INCLUDED
