#pragma once
#include "HttpHeaders.h"

#ifndef HTTPREQUEST_MAX_MESSAGE_SIZE
#define HTTPREQUEST_MAX_MESSAGE_SIZE 512
#endif

#ifdef _TEST_
#include "Stream.h"
#else
#include "Arduino.h"
#endif

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
        const char * setMessage(const char *, size_t);
        const char * getMessage();
        char method [HTTPREQUEST_METHOD_SIZE];
        char * url;
        char httpver [HTTPREQUEST_HTTPVER_SIZE];
        HttpHeaders headers;
        long int message_length;
        char * message;
};
