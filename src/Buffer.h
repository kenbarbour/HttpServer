#pragma once
#include "stdint.h"
#include "stdlib.h"

#ifdef _TEST_
#include "dummy/DummyStream.h"
#else
#include "Arduino.h"
#endif

class Buffer: public Stream
{
    public:
        Buffer(uint8_t*, size_t);
        virtual size_t availableToWrite();
        virtual int available();
        virtual size_t write(uint8_t);
        virtual size_t write(uint8_t *, size_t);
        virtual int read();
        virtual int peek();
    
    private:
        uint8_t * start;
        uint8_t * r_ptr;
        uint8_t * w_ptr;
        size_t size;
        size_t num;
};
