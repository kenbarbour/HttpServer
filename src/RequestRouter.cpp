#include "RequestRouter.h"

#include <iostream>

RequestRouter::RequestRouter(const Route * routes, size_t num): routes(routes), num(num) {};

const Route* RequestRouter::match(HttpRequest &request)
{
    bool urlMatch = false; // if true 
    for (int i = 0; i < num; i++) {
        if (!urlMatches(routes[i].pattern, request.getUrl()))
            continue;
        urlMatch = true;
        if (methodMatches(routes[i].methods, request.getMethod()))
            return &(routes[i]);
    }

    return nullptr;

    // todo: handle methodNotAllowed and NotFound
}

bool RequestRouter::urlMatches(const char * pattern, const char * url)
{
    // todo stub
    return false;
}

bool RequestRouter::methodMatches(uint8_t pattern, const char * method)
{
    // todo stub
    return false;
}

RequestRouter::~RequestRouter() 
{}
