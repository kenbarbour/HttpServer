#include "RouteDispatcher.h"

RouteDispatcher::RouteDispatcher(RequestRouter& router)
    : router(router), 
      notFoundHandler(handleNotFound),
      methodNotAllowedHandler(handleMethodNotAllowed)
{
}

void RouteDispatcher::handle(HttpRequest& request, HttpResponse& response)
{
    const Route * route = router.match(request);
    if (route == nullptr) switch (router.getRouteError()) {
        case E_NOT_FOUND:
            notFoundHandler(request, response);
            break;
        case E_METHOD_NOT_ALLOWED:
            responseSetAllowedHeader(response, router.lastAllowedMethods());
            methodNotAllowedHandler(request, response);
            break;
        default:
            response.code = 500;
            break;
    } else {
        route->handler(request, response);
    }
}

void RouteDispatcher::handleNotFound(HttpRequest& request, HttpResponse& response)
{
    response.code = 404;
    response.headers.set("Content-Type","text/plain");
    response.print(response.getReason());
}

void RouteDispatcher::handleMethodNotAllowed(HttpRequest& request, HttpResponse& response)
{
    response.code = 405;
    response.headers.set("Content-Type","text/plain");
    response.print(response.getReason());
}

void RouteDispatcher::responseSetAllowedHeader(HttpResponse& response, uint8_t allowed)
{
    if (allowed & GET)
        response.headers.append("Allow","GET");
    if (allowed & POST)
       response.headers.append("Allow","POST");
    if (allowed & PUT)
        response.headers.append("Allow","PUT");
    if (allowed & DELETE)
        response.headers.append("Allow","DELETE");
}
