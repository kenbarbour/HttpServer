#pragma once
#include "HttpHeaders.h"

#ifdef _TEST_
#include "dummy/DummyStream.h"
#else
#include "Arduino.h"
#endif

class HttpResponse
{
    public:
        HttpResponse();
        HttpResponse(unsigned int code);
        HttpResponse(unsigned int code, const char *);
        ~HttpResponse();
        unsigned int code;
        char * reason;
        HttpHeaders headers;
        Stream * message;
        const char * setReason(const char * reason);
        const char * getReason();
        static const char * getDefaultReason(unsigned int);
    private:
};
