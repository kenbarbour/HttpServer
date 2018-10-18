#pragma once
#include "Print.h"
#include "Printable.h"

#ifndef HTTPHEADERS_BUFFER
#define HTTPHEADERS_BUFFER 256
#endif

#ifndef HTTPHEADERS_NUM
#define HTTPHEADERS_NUM 16
#endif

#define HTTPHEADERS_NAME_SZ 24
#define HTTPHEADERS_VALUE_SZ 104

struct HttpHeaderField {
    char name[HTTPHEADERS_NAME_SZ];
    char value[HTTPHEADERS_VALUE_SZ];
};

class HttpHeaders: public Printable {
    public:
        HttpHeaders():
            _headers(),
            _num(0)
            {};

        void set(const char * , const char *);
        void append(const char *, const char *);
        char* get(const char*); //TODO make const
        bool has(const char*);
        bool in(const char*, const char*);

        unsigned int count();
        unsigned int length();
        virtual size_t printTo(Print&) const;

    private:
        HttpHeaderField _headers[HTTPHEADERS_NUM];
        unsigned int _num;
        int indexof( const char *);
        bool names_match(const char *, const char *);
};
