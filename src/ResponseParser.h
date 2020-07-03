#pragma once
#include "HttpResponse.h"
#include "Arduino.h"
#include "Stream.h"

#ifndef RESPONSEPARSER_BUFFER
#define RESPONSEPARSER_BUFFER 512
#endif

enum ResponseParserState {
    S_IN_HTTPVER,
    S_IN_CODE,
    S_IN_REASON,
    S_IN_HEADER,
    S_IN_MESSAGE,
    S_COMPLETE,
    S_ERROR
};
    
class ResponseParser
{
    public:
        ResponseParser(HttpResponse& res, Stream& server):
            response(res),
            server(server),
            _state(S_IN_HTTPVER)
            {};

        bool parse();
        bool error() { return _state == S_ERROR; };
        void reset();
        

    private:
        char _buffer[RESPONSEPARSER_BUFFER] = {};
        char * _i = _buffer;
        HttpResponse& response;
        Stream& server;
        ResponseParserState _state;
};
