#include "RouteDispatcher.h"

RouteDispatcher::RouteDispatcher(RequestRouter& router)
    : router(router)
{
}

void RouteDispatcher::handle(HttpRequest& request, HttpResponse& response)
{
    try {
        const Route * route = router.match(request);
        route->handler(request, response);
    } catch (NotFoundException &e) {
        response.code = 404;
    } catch (MethodNotAllowedException &e) {
        response.code = 405;
        if (e.methodsAllowed & GET == GET)
            response.headers.append("Allow","GET");
        if (e.methodsAllowed & POST == POST)
            response.headers.append("Allow","POST");
        if (e.methodsAllowed & PUT == PUT)
            response.headers.append("Allow","PUT");
        if (e.methodsAllowed & DELETE == DELETE)
            response.headers.append("Allow","DELETE");
    }
}
