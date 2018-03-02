#pragma once
#include "RequestRouter.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class RouteDispatcher
{
    public:
        RouteDispatcher(RequestRouter&);
        void handle(HttpRequest& , HttpResponse&);
        
        // Default handlers for some common http errors
        static void handleNotFound(HttpRequest&, HttpResponse&);
        static void handleMethodNotAllowed(HttpRequest&, HttpResponse&, uint8_t);
        
        // re-assignable pointers to error handlers
        void (*notFoundHandler)(HttpRequest&, HttpResponse&);
        void (*methodNotAllowedHandler)(HttpRequest&, HttpResponse&, uint8_t);

    private:
        RequestRouter& router;
};
