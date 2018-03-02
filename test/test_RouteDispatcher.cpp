#include "catch.hpp"
#include "RouteDispatcher.h"

using Catch::Matchers::Equals;

void dummy_handler(HttpRequest& request, HttpResponse& response)
{
    response.code = 202;
    response.setReason("I said so");
}

void dummy_notFoundHandler(HttpRequest& request, HttpResponse& response)
{
    response.code = 404;
    response.setReason("Custom");
}

void dummy_methodNotAllowedHandler(HttpRequest& request, HttpResponse& response, uint8_t allowed)
{
    response.code = 405;
    response.setReason("custom_not_allowed");
}

TEST_CASE("Test handle","[RouteDispatcher]")
{
    Route routes[] = {
            {GET, "/", dummy_handler}
        };
    RequestRouter router(routes,1);
    RouteDispatcher dispatcher(router);
    HttpRequest request;
    request.setMethod("GET");
    request.setUrl("/");
    HttpResponse response;
    
    dispatcher.handle(request, response);

    CHECK(response.code != 404);
    CHECK(response.code != 405);
    CHECK(response.code == 202);
    CHECK_THAT(response.getReason(), Equals("I said so"));
}

TEST_CASE("Test default methodNotAllowedHandler","[RouteDispatcher]")
{
    Route routes[] = {
            { GET, "/", dummy_handler }
        };

    RequestRouter router(routes,1);
    RouteDispatcher dispatcher(router);
    dispatcher.notFoundHandler = dummy_notFoundHandler;
    HttpRequest request;
    request.setMethod("POST");
    request.setUrl("/");
    HttpResponse response;

    dispatcher.handle(request, response);

    CHECK_THAT(response.getReason(), Equals("Method Not Allowed"));
    CHECK(response.code == 405);
    CHECK_THAT(response.headers.get("Allow"), Equals("GET"));

}

TEST_CASE("Test notFoundHandler","[RouteDispatcher]")
{
    Route routes[] = {
            { GET, "/", dummy_handler }
        };

    RequestRouter router(routes,1);
    RouteDispatcher dispatcher(router);
    dispatcher.notFoundHandler = dummy_notFoundHandler;
    HttpRequest request;
    request.setMethod("GET");
    request.setUrl("/foo");
    HttpResponse response;

    dispatcher.handle(request, response);

    CHECK_THAT(response.getReason(), Equals("Custom"));
}

TEST_CASE("Test methodNotAllowedHandler","[RouteDispatcher]")
{
    Route routes[] = {
            { GET, "/", dummy_handler }
        };

    RequestRouter router(routes,1);
    RouteDispatcher dispatcher(router);
    dispatcher.methodNotAllowedHandler = dummy_methodNotAllowedHandler;
    HttpRequest request;
    request.setMethod("POST");
    request.setUrl("/");
    HttpResponse response;

    dispatcher.handle(request, response);

    CHECK_THAT(response.getReason(), Equals("custom_not_allowed"));

}
