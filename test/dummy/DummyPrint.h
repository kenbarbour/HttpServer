#ifndef DUMMYPRINT_H
#define DUMMYPRINT_H

#include "Arduino.h"

class DummyPrint
{
    public:
        DummyPrint(char * buff): buffer(buff), pos(0) {};
        virtual size_t write(uint8_t c) { buffer[pos++]=(char) c; buffer[pos]='\0';return sizeof(c); }
        int pos;
        char * buffer;
};

extern Print

#endif // DUMMYPRINT_H
