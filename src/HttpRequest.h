#pragma once
#include "HttpHeaders.h"

#ifndef HTTPREQUEST_TIMEOUT
#define HTTPREQUEST_TIMEOUT 1000
#endif

#ifndef HTTPREQUEST_MAX_MESSAGE_SIZE
#define HTTPREQUEST_MAX_MESSAGE_SIZE 512
#endif

#ifdef _TEST_
#include "Stream.h"
#endif

#include "Arduino.h"

#define HTTPREQUEST_METHOD_SIZE 8
#define HTTPREQUEST_HTTPVER_SIZE 9

class HttpRequest
{
    public:
        HttpRequest();
        HttpRequest(Stream&);
        ~HttpRequest();
        void capture(Stream&);
        const char * setMethod(const char *);
        const char * getMethod();
        const char * setUrl(const char *);
        const char * setUrl(const char *, size_t);
        const char * getUrl();
        const char * setHttpVer(const char *);
        const char * getHttpVer();
        const char * setMessage(const char *);
        const char * setMessage(const char *, unsigned int);
        const char * getMessage();
        unsigned int getMessageLength() { return message_length; };
        char method [HTTPREQUEST_METHOD_SIZE];
        char * url;
        char httpver [HTTPREQUEST_HTTPVER_SIZE];
        HttpHeaders headers;
    protected:
        char client_read(Stream&);
        long int message_length;
        char * message;
        char client_peek(Stream&);
        unsigned long _timeout_millis;
};
