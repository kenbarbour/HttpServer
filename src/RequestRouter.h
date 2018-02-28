#pragma once
#include "Methods.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Methods.h"

typedef struct Route
{
    uint8_t methods;
    const char * pattern;
    void (*handler) (HttpRequest&, HttpResponse&);
} Route;

typedef enum RouteError
{
    E_SUCCESS = 0,
    E_NOT_FOUND = 1,
    E_METHOD_NOT_ALLOWED = 2
} RouteError;

class RequestRouter
{
    public:
        RequestRouter(const Route *, size_t);
        ~RequestRouter();
        const Route* match(HttpRequest &request);
        const Route* routes;
        static bool urlMatches(const char * pattern, const char * url);
        RouteError getRouteError() { return route_error; }
        void clearRouteError() { setRouteError(E_SUCCESS); }
    protected:
        void setRouteError(RouteError err) { route_error = err; }
    private:
        size_t num;
        RouteError route_error;
        bool methodMatches(uint8_t pattern, uint8_t method);
        uint8_t allowedMethods(const char * url);
};
