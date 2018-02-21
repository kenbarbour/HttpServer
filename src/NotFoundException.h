#pragma once
#include <exception>
#include "HttpRequest.h"

class NotFoundException: public std::exception
{
    public:
        NotFoundException(HttpRequest * request)
            :request(request) {};
        HttpRequest * request; 
};
