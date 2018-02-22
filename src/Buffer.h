#pragma once
#include "stdint.h"
#include "stdlib.h"

class Buffer
{
    public:
        Buffer(uint8_t*, size_t);
        size_t availableToWrite();
        size_t available();
        size_t write(uint8_t);
        size_t write(uint8_t *, size_t);
        uint8_t read();
    
    private:
        uint8_t * start;
        uint8_t * r_ptr;
        uint8_t * w_ptr;
        size_t size;
        size_t num;
};
