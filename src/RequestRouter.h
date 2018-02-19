#pragma once
#include "Methods.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

typedef struct Route
{
    uint8_t methods;
    const char * pattern;
    //HttpResponse (*handler) (HttpRequest);
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
        bool methodMatches(uint8_t pattern, const char * method);
};
