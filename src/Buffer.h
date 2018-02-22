#pragma once

#ifdef _TEST_
#include "dummy/DummyStream.h"
#else
#include "Arduino.h"
#endif

class Buffer: public Stream
{
    public:
        Buffer(uint8_t*, size_t);
        size_t availableToWrite();
        int available();
        size_t write(uint8_t);
        size_t write(const uint8_t *, size_t);
        size_t write(const char * buffer) {
            return write((const uint8_t *)buffer, strlen(buffer));
        }
        size_t write(const char * buffer, size_t size) {
            return write((const uint8_t *)buffer, size);
        }
        int read();
        int peek();

        virtual void flush();
    
    private:
        uint8_t * start;
        uint8_t * r_ptr;
        uint8_t * w_ptr;
        size_t size;
        size_t num;
};
