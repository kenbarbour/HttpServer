#pragma once
#include "Stream.h"
#include "Buffer.h"

#define WIFICLIENT_BUFFER 512

class WiFiClient : public Stream
{
    public:
        WiFiClient(const char * content): buffer(_buff, WIFICLIENT_BUFFER), _status(1) {
            buffer.write(content);
        }
        WiFiClient(): buffer(_buff, WIFICLIENT_BUFFER), _status(1) {}

        uint8_t _status;

        void stop() { _status=0; }
        uint8_t connected() { return _status; }

        int available() { return buffer.available(); }
        int read() { return buffer.read(); }
        int peek() { return buffer.peek(); }

        size_t write(uint8_t data) { return buffer.write(data); }
        size_t write(const uint8_t* buf, size_t len) { return buffer.write(buf, len); }

        int availableForWrite() { return buffer.availableForWrite(); }
        using Stream::write; 
    protected:
        Buffer buffer;
    private:
        uint8_t _buff[WIFICLIENT_BUFFER];
};
