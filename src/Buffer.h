#pragma once

#ifdef _TEST_
#include "Stream.h"
#else
#include "Arduino.h"
#endif
#include "Printable.h"

class Buffer: public Stream, public Printable
{
    public:
        Buffer(uint8_t*, size_t);
        virtual size_t write(uint8_t);
        int availableForWrite();
        int available() const;
        int available();
        int peek();
        int read();
        void clear();
        using Print::write;
        size_t printTo(Print& stream) const;
    
    private:
        uint8_t * start;
        uint8_t * r_ptr;
        uint8_t * w_ptr;
        size_t size;
        size_t num;
};
