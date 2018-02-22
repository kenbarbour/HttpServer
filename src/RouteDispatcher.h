#pragma once
#include "RequestRouter.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class RouteDispatcher
{
    public:
        RouteDispatcher(RequestRouter&);
        void handle(HttpRequest& , HttpResponse&);
    private:
        RequestRouter& router;
};
