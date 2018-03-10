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

class HttpResponse: public Print, public Printable
{
    public:
        HttpResponse(Buffer&);
        HttpResponse(Buffer&, unsigned int);
        HttpResponse(Buffer&, unsigned int, const char *);
        HttpResponse();
        HttpResponse(unsigned int code);
        HttpResponse(unsigned int code, const char *);
        ~HttpResponse();
        unsigned int code;
        HttpHeaders headers;
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
};
