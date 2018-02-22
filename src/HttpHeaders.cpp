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

void HttpHeaders::set(const char * name, const char * value)
{
    int name_len = strlen(name);
    int value_len = strlen(value);

    for (int i = 0; i < count(); i++) {
        if (strcmp(name, headers[i].name) != 0)
            continue;
        headers[i].value = (char *) realloc(headers[i].value, value_len + 1);
        strcpy(headers[i].value, value);
        return;
    }

    // Grow headers if needed
    if (_n && (_n % HTTPHEADERS_SIZE) == 0) {
        headers = (HttpHeaderNode*) realloc(headers, sizeof(HttpHeaderNode) * HTTPHEADERS_SIZE * (_n / HTTPHEADERS_SIZE + 1));
    }

    headers[_n].name = (char *) malloc( name_len + 1);
    headers[_n].value = (char *) malloc( value_len + 1);
    strcpy(headers[_n].name, name);
    strcpy(headers[_n].value, value);
    _n++;
}

void HttpHeaders::append(const char* name, const char* value) {

    int name_len = strlen(name);
    int value_len = strlen(value);

    // search for existing node with name and append value
    for (int i = 0; i < count(); i++) {
        if (strcmp(name, headers[i].name) != 0)
            continue;
        int old_value_len = strlen(headers[i].value);
        headers[i].value = (char *) realloc(headers[i].value, old_value_len + value_len + 3);
        strcpy(headers[i].value+old_value_len, ", ");
        strcpy(headers[i].value+old_value_len+2, value);
        return;
    }

    // Grow headers if needed
    if (_n && (_n % HTTPHEADERS_SIZE) == 0) {
        headers = (HttpHeaderNode*) realloc(headers, sizeof(HttpHeaderNode) * HTTPHEADERS_SIZE * (_n / HTTPHEADERS_SIZE + 1));
    }

    // create and store new node with name and value
    headers[_n].name  = (char *) malloc(name_len + 1);
    headers[_n].value = (char *) malloc(value_len + 1);
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
    if (headers) {
        free(headers);
        headers = nullptr;
    }
}
