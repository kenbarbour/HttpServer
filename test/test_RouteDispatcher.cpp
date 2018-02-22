#include "catch.hpp"
#include "RouteDispatcher.h"

using Catch::Matchers::Equals;

void dummy_handler(HttpRequest& request, HttpResponse& response)
{
    response.code = 202;
    response.setReason("I said so");
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
