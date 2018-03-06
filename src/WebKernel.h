#pragma once
#include "WiFiServer.h"
#include "RequestRouter.h"
#include "RouteDispatcher.h"

class WebKernel
{
    public:
        WebKernel(uint16_t port, Route* routes, uint8_t num):
            _server(port),
            _router(routes, num)
            {};
        void begin() { _server.begin(); }

#ifdef _TEST_
        void mock_nextClient(const char * next) { _server._next = next; }
#endif

    protected:
        WiFiServer _server; 
        RequestRouter _router;
};
