#include "RequestRouter.h"
#include <string.h>

RequestRouter::RequestRouter(const Route * routes, size_t num): routes(routes), num(num), route_error(E_SUCCESS) {};

const Route* RequestRouter::match(HttpRequest &request)
{
    bool urlMatch = false; // if true
    uint8_t method = method_str(request.getMethod());
    allowed_methods = 0;

    for (int i = 0; i < num; i++) {
        if (urlMatches(routes[i].pattern, request.getUrl())) {
            urlMatch = true;
            allowed_methods = allowed_methods | routes[i].methods;
            if (methodMatches(routes[i].methods, method)) {
                clearRouteError();
                return &(routes[i]);
            }
        }
    }

    if (urlMatch) {
        setRouteError(E_METHOD_NOT_ALLOWED);
        return nullptr;
    } 

    setRouteError(E_NOT_FOUND);
    return nullptr;

}

bool RequestRouter::urlMatches(const char * pattern, const char * url)
{
    unsigned int u = 0;
    unsigned int p = 0;
    
    do {
        if (pattern[p] == url[u] && url[u] == '\0')
            return true;

        if (pattern[p] == '*') {
            if (url[u] == '/') p++;
            else if (url[u] == '\0') p++;
            else u++;
        } else {
            if (pattern[p] == url[u]) {
                p++;
                u++;
                continue;
            }
            if (pattern[p] == '\0' && url[u] == '?')
                return true;
            if (pattern[p] == '#') return true;
            return false;
        }

    } while (true);

    return true;
}

bool RequestRouter::methodMatches(uint8_t pattern, uint8_t method)
{
    return (pattern & method) == method;
}

RequestRouter::~RequestRouter() 
{}
