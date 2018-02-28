#include "RouteDispatcher.h"

RouteDispatcher::RouteDispatcher(RequestRouter& router)
    : router(router)
{
}

void RouteDispatcher::handle(HttpRequest& request, HttpResponse& response)
{
    const Route * route = router.match(request);
    if (route == nullptr) switch (router.getRouteError()) {
        case E_NOT_FOUND:
            handleNotFoundError(request, response);
            break;
        case E_METHOD_NOT_ALLOWED:
            handleMethodNotAllowedError(request, response);
            break;
        default:
            response.code = 500;
            break;
    } else {
        route->handler(request, response);
    }
}

void RouteDispatcher::handleNotFoundError(HttpRequest& request, HttpResponse& response)
{
    response.code = 404;
    // TODO: finish handling Not Found Error
}

void RouteDispatcher::handleMethodNotAllowedError(HttpRequest& request, HttpResponse& response)
{
    response.code = 405;

    /* // TODO: transform old exception code to determine allowed methods
    if (e.methodsAllowed & GET == GET)
        response.headers.append("Allow","GET");
    if (e.methodsAllowed & POST == POST)
       response.headers.append("Allow","POST");
    if (e.methodsAllowed & PUT == PUT)
        response.headers.append("Allow","PUT");
    if (e.methodsAllowed & DELETE == DELETE)
        response.headers.append("Allow","DELETE");
    */
    // TODO: finish handling Method Not Allowed Error 
}
