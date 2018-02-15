#pragma once

#include "Arduino.h"

class DummyStream: public Stream
{
    public:
        DummyStream(char* buff): buffer(buffer), pos(0) {};
        char * buffer;
        int pos;
        virtual int available() { return (buffer[pos] != '\0'); }
        virtual int read() { return buffer[pos++]; }
        virtual int peek() { return buffer[pos]; }
        virtual size_t write(uint8_t c) { return 0; } // gone forever
};

extern DummyStream Stream
