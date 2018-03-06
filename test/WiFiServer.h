#pragma once
#include "WiFiClient.h"

class WiFiServer
{
    public:
        WiFiServer(uint16_t port) : port(port) {}
        WiFiClient available() {
            if (!_next) return WiFiClient();
            const char * client = _next;
            _next = nullptr;
            return WiFiClient(client);
        };
        void begin() {};

        uint16_t port;
        const char * _next;
};
