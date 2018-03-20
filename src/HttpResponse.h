#pragma once
#include "HttpHeaders.h"
#include "Buffer.h"

#ifdef _TEST_
#include "Stream.h"
#else
#include "Arduino.h"
#endif

#include "Print.h"
#include "Printable.h"

#define HTTPRESPONSE_HTTPVER_SIZE 9

class HttpResponse: public Print, public Printable
{
    public:
        HttpResponse(Buffer&); // TODO: only cnstr needed by WebKernel, add httpver param
        HttpResponse(Buffer&, unsigned int);  // TODO: remove unnecessary constructors
        HttpResponse(Buffer&, unsigned int, const char *);
        HttpResponse();
        HttpResponse(unsigned int code);
        HttpResponse(unsigned int code, const char *);
        ~HttpResponse();
        unsigned int code;
        HttpHeaders headers;
        const char * setHttpVersion(const char *);
        const char * getHttpVersion() const;
        const char * setReason(const char * reason);
        const char * getReason() const;
        static const char * getDefaultReason(unsigned int);
        void send(Stream &client);
        size_t write (uint8_t);
        size_t write (uint8_t *, size_t);
        using Print::write;

        size_t printTo(Print&) const;
    private:
        Buffer * buffer;
        char * reason;
        char httpver[HTTPRESPONSE_HTTPVER_SIZE];
};
