#pragma once
#include "Arduino.h"
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "RequestParser.h"
#include "RequestRouter.h"
#include "RouteDispatcher.h"

#define WEBKERNEL_MAX_WAIT 1000

#define WEBKERNEL_RESPONSE_SIZE 1024

enum WebKernelState {
    S_IDLE,
    S_RECEIVING,
    S_DISPATCHING
};

class WebKernel
{
    public:
        WebKernel(uint16_t port, Route* routes, uint8_t num):
            _server(port),
            _client(),
            _request(),
            _router(routes, num),
            _state(S_IDLE),
            _dispatcher(_router),
            _parser(_request, _client),
            _resp_buffer(_resp_data, WEBKERNEL_RESPONSE_SIZE)
            {};
        void begin() { _server.begin(); }
        void handleClients();

#ifdef _TEST_
        void mock_nextClient(const char * next) { _server._next = next; }
        WiFiClient& mock_currentClient() { return _client; }
#endif

    protected:

        WiFiServer _server;
        WiFiClient _client; 
        HttpRequest _request;
        RequestRouter _router;
        RequestParser _parser;
        RouteDispatcher _dispatcher;
        WebKernelState _state;
        unsigned long int _stateChange;

        uint8_t _resp_data[WEBKERNEL_RESPONSE_SIZE];
        Buffer _resp_buffer;
};
