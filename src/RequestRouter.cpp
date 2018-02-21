#include "RequestRouter.h"
#include "MethodNotAllowedException.h"
#include "NotFoundException.h"
#include <string.h>

RequestRouter::RequestRouter(const Route * routes, size_t num): routes(routes), num(num) {};

const Route* RequestRouter::match(HttpRequest &request)
{
    bool urlMatch = false; // if true
    uint8_t method = method_str(request.getMethod());
    uint8_t allowedMethods = 0;

    for (int i = 0; i < num; i++) {
        if (urlMatches(routes[i].pattern, request.getUrl())) {
            urlMatch = true;
            allowedMethods = allowedMethods | routes[i].methods;
            if (methodMatches(routes[i].methods, method))
                return &(routes[i]);
        }
    }

    if (urlMatch) {
        throw MethodNotAllowedException(&request, allowedMethods);
    } 

    throw NotFoundException(&request);

}

bool RequestRouter::urlMatches(const char * pattern, const char * url)
{
    return (strcmp(pattern, url) == 0);
}

bool RequestRouter::methodMatches(uint8_t pattern, uint8_t method)
{
    return (pattern & method) == method;
}

RequestRouter::~RequestRouter() 
{}
