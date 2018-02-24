#pragma once
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

class Print
{
    public:
        Print() {}
        virtual size_t write(uint8_t) = 0;
        size_t write(const char *str) {
        if (str == NULL) return 0;
            return write((const uint8_t *)str, strlen(str));
        }
        virtual size_t write(const uint8_t *buffer, size_t size);
        size_t write(const char *buffer, size_t size) {
            return write((const uint8_t *)buffer, size);
        }
        virtual int availableForWrite() { return 0; }

        virtual void flush() {}
};