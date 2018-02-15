#pragma once
#include "HttpHeaders.h"

#ifndef HTTPREQUEST_MAX_MESSAGE_SIZE
#define HTTPREQUEST_MAX_MESSAGE_SIZE 512
#endif

#ifdef _TEST_
#include "dummy/DummyStream.h"
#else
#include "Arduino.h"
#endif

#define HTTPREQUEST_METHOD_SIZE 8
#define HTTPREQUEST_HTTPVER_SIZE 9

class HttpRequest
{
    public:
        HttpRequest(Stream& s)
            :client(s), method(), url(), httpver(), headers(), message() {};
        ~HttpRequest();
        Stream& client;
        void capture();
        char method [HTTPREQUEST_METHOD_SIZE];
        char * url;
        char httpver [HTTPREQUEST_HTTPVER_SIZE];
        HttpHeaders headers;
        long int message_length;
        char * message;
};
