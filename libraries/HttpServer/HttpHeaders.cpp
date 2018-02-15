#include "HttpHeaders.h"
#include "stdlib.h"
#include "string.h"
#include <iostream>


#ifndef HTTPHEADERS_SIZE
#define HTTPHEADERS_SIZE 20
#endif


HttpHeaders::HttpHeaders() : _n(0) {
    headers = (HttpHeaderNode*) malloc(sizeof(HttpHeaderNode) * HTTPHEADERS_SIZE);
}

void HttpHeaders::set(const char* name, const char* value) {
    for (int i = 0; i < count(); i++) {
        if (strcmp(name, headers[i].name) != 0)
            continue;
        free(headers[i].value);
        headers[i].value = (char *) malloc(sizeof(char) * strlen(value));
        strcpy(headers[i].value, value);
        return;
    }
    headers[_n].name  = (char *) malloc(sizeof(char) * strlen(name));
    headers[_n].value = (char *) malloc(sizeof(char) * strlen(value));
    strcpy(headers[_n].name, name);
    strcpy(headers[_n].value, value);
    _n++;
}

char* HttpHeaders::get(const char* name)
{
    for (int i = 0; i < count(); i++) {
        if (strcmp(name, headers[i].name) == 0)
            return headers[i].value;
    }
    return nullptr;
}

bool HttpHeaders::has(const char* name)
{
    for (int i = 0; i < count(); i++) {
        if (strcmp(name, headers[i].name) == 0)
            return true;
    }
    return false;
}

int HttpHeaders::count()
{
    return _n;
}

int HttpHeaders::length()
{
    int length = 0;
    for (int i = 0; i < count(); i++) {
        length += strlen(headers[i].name);
        length += strlen(headers[i].value);
        length += 4; // add ': ' and '\r\n'
    }
    return length;
}

HttpHeaders::~HttpHeaders() {
    for (int i = 0; i < count(); i++) {
        free(headers[i].name);
        free(headers[i].value);
    }
    free(headers);
}
