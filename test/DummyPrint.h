#pragma once
#include "Print.h"

class DummyPrint: public Print
{
    public:
        DummyPrint(char * buff): buffer(buff), pos(0) {};
        size_t write(uint8_t c) { buffer[pos++]=(char) c; buffer[pos]='\0';return 1; }
        int pos;
        char * buffer;
};

