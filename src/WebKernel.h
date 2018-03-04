#pragma once
#include "WiFiServer.h"

class WebKernel
{
    public:
        WebKernel(uint16_t port):
            _server(port)
            {};
        WiFiServer getServer() { return _server; } 

    protected:
        WiFiServer _server; 
};
