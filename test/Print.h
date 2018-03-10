#pragma once
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "Printable.h"

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

        size_t print(const char* str) { return write(str); }
        size_t print(char c ) { return write(c); }
        size_t print(int);
        size_t print(const Printable&);

        size_t println(void);
        size_t println(const char* );
        size_t println(char);
        size_t println(int);
        size_t println(const Printable&);

        size_t printNumber(unsigned long n, uint8_t base);
};
