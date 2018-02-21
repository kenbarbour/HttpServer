#pragma once
#include <exception>
#include "HttpRequest.h"

class MethodNotAllowedException: public std::exception
{
    public:
        MethodNotAllowedException(HttpRequest * request, uint8_t allowed)
            : request(request), methodsAllowed(allowed) {};
        HttpRequest * request;
        uint8_t methodsAllowed;
};
