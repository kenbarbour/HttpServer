#pragma once
#include "HttpRequest.h"
#include "Arduino.h"
#include "Stream.h"

#ifndef REQUESTPARSER_BUFFER
#define REQUESTPARSER_BUFFER 512
#endif

enum RequestParserState {
    S_IN_METHOD,
    S_IN_URL,
    S_IN_HTTPVER,
    S_IN_HEADER,
    S_IN_MESSAGE,
    S_COMPLETE,
    S_ERROR
};
    
class RequestParser
{
    public:
        RequestParser(HttpRequest& req, Stream& client):
            request(req),
            client(client),
            _state(S_IN_METHOD)
            {};

        bool parse();
        bool error() { return _state == S_ERROR; };
        void reset();
        

    private:
        char _buffer[REQUESTPARSER_BUFFER] = {};
        char * _i = _buffer;
        HttpRequest& request;
        Stream& client;
        RequestParserState _state;
};
