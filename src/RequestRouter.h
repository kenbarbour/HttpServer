#pragma once
#include "Methods.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Methods.h"
#include "NotFoundException.h"
#include "MethodNotAllowedException.h"

typedef struct Route
{
    uint8_t methods;
    const char * pattern;
    void (*handler) (HttpRequest&, HttpResponse&);
} Route;

class RequestRouter
{
    public:
        RequestRouter(const Route *, size_t);
        ~RequestRouter();
        const Route* match(HttpRequest &request);
        const Route* routes;
    private:
        size_t num;
        bool urlMatches(const char * pattern, const char * url);
        bool methodMatches(uint8_t pattern, uint8_t method);
        uint8_t allowedMethods(const char * url);
};
