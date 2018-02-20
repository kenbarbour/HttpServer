#pragma once
#include <exception>
#include "HttpRequest.h"

class MethodNotAllowedException: public std::exception
{
    public:
        MethodNotAllowedException(uint8_t allowed)
            : methodsAllowed(allowed) {};
        uint8_t methodsAllowed;
};
