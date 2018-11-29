#pragma once
#include "HttpHeaders.h"

#ifndef HTTPREQUEST_TIMEOUT
#define HTTPREQUEST_TIMEOUT 1000
#endif

#ifndef HTTPREQUEST_MAX_MESSAGE_SIZE
#define HTTPREQUEST_MAX_MESSAGE_SIZE 512
#endif

#include "Arduino.h"

#define HTTPREQUEST_METHOD_SIZE 8
#define HTTPREQUEST_HTTPVER_SIZE 9

class HttpRequest
{
    public:
        HttpRequest();
        ~HttpRequest();
        const char * setMethod(const char *);
        const char * getMethod() const;
        const char * setUrl(const char *);
        const char * setUrl(const char *, size_t);
        const char * getUrl() const;
        const char * setHttpVersion(const char *);
        const char * getHttpVersion() const;
        const char * setMessage(const char *);
        const char * setMessage(const char *, unsigned int);
        const char * getMessage() const;
        unsigned int getMessageLength() { return message_length; };
        HttpRequest& operator=(const HttpRequest& other);
        char method [HTTPREQUEST_METHOD_SIZE];
        char * url;
        char httpver [HTTPREQUEST_HTTPVER_SIZE]; // TODO: private
        HttpHeaders headers;
    protected:
        long int message_length;
        char * message;
};
