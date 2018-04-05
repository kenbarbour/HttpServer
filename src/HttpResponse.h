#pragma once
#include "HttpHeaders.h"
#include "Buffer.h"

#ifdef _TEST_
#include "Stream.h"
#else
#include "Arduino.h"
#endif

#include "Printable.h"

#define HTTPRESPONSE_HTTPVER_SIZE 9

class HttpResponse
{
    public:
        HttpResponse(Stream&); // TODO: only cnstr needed by WebKernel, add httpver param
        HttpResponse();
        ~HttpResponse();
        unsigned int code;
        HttpHeaders headers;
        const char * setHttpVersion(const char *);
        const char * getHttpVersion() const;
        const char * setReason(const char * reason);
        const char * getReason() const;
        static const char * getDefaultReason(unsigned int);
        Stream * content;

        size_t send(Print&);

    private:
        char * reason;
        char httpver[HTTPRESPONSE_HTTPVER_SIZE];
};
