#include "HttpHeaders.h"
#include "stdlib.h"
#include "string.h"

void HttpHeaders::set(const char * name, const char * value)
{
    int i = indexof(name);
    if (i == -1) {
        i = _num++;
        strncpy(_headers[i].name, name, HTTPHEADERS_NAME_SZ -1);
    }
    strncpy(_headers[i].value, value, HTTPHEADERS_VALUE_SZ -1);
}

void HttpHeaders::append(const char * name, const char * value)
{
    int i = indexof(name);
    if (i == -1) return set(name, value);

    int val_len = strlen(_headers[i].value);
    if (val_len == HTTPHEADERS_VALUE_SZ - 3) return;
    _headers[i].value[val_len++] = ',';
    _headers[i].value[val_len++] = ' ';
    strncpy(_headers[i].value + val_len, value, HTTPHEADERS_VALUE_SZ - val_len - 1);
}

char* HttpHeaders::get(const char * name) // TODO make const
{
    int i = indexof(name);
    if (i == -1) return nullptr;
    return _headers[i].value;
}

bool HttpHeaders::has(const char* name)
{
    return (indexof(name) != -1);
}

unsigned int HttpHeaders::count()
{
    return _num;
}

unsigned int HttpHeaders::length()
{
    unsigned int len = 0;
    for (int i = 0; i < _num; i++) {
        len += strlen(_headers[i].name);
        len += strlen(_headers[i].value);
    }
    len += 2*_num; // count the ': ' between name and value
    return len;
}

size_t HttpHeaders::printTo(Print& client) const
{
    size_t len = 0;
    for (int i = 0; i < _num; i++) {
        len += client.print(_headers[i].name);
        len += client.print(": ");
        len += client.println(_headers[i].value);
    }
    return len;
}

int HttpHeaders::indexof( const char * name)
{
    for (int i = 0; i < _num; i++) {
        if (names_match(name, _headers[i].name)) return i;
    }
    return -1;
}

bool HttpHeaders::names_match(const char * a, const char * b)
{
    char ac,bc;
    for (int i = 0; ac=a[i]; i++) {
        bc = b[i];
        if (ac == bc) continue;
        if (ac > 96 && ac < 123) ac -= 32;
        if (bc > 96 && bc < 123) bc -= 32;
        if (ac != bc) return false;
    }

    return true;
}
