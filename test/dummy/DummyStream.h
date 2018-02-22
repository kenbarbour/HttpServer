#pragma once
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "dummy/DummyPrint.h"

class Stream: public Print
{
    public:
        virtual int available() = 0;
        virtual int read() = 0;
        virtual int peek() = 0;
        virtual size_t write(uint8_t) = 0;
};

class DummyStream: public Stream
{
    public:
        DummyStream(const char* buff): buffer(buff), pos(0) {};
        const char * buffer;
        int pos;
        virtual int available() { return (strlen(buffer) - pos); }
        virtual int read() { return buffer[pos++]; }
        virtual int peek() { return buffer[pos]; }
        virtual size_t write(uint8_t c) { return 0; } // gone forever
};
