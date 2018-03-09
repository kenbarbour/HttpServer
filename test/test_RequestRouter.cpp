#include "RequestRouter.h"
#include "catch.hpp"
#include "DummyStream.h"
#include <iostream>
#include <bitset>

using Catch::Matchers::Equals;

TEST_CASE("Test Route struct","[RequestRouter][Route]")
{
    Route rt = {
            GET | POST,
            "/foo",
            //nullptr
        };
}

TEST_CASE("test constructor","[RequestRouter]")
{
    Route routes[] = {
            { GET, "/" },
            { GET, "/foo" },
            { POST, "/foo" },
            { GET|POST, "/bar" }
        };
    RequestRouter router(routes, 4);
    for (int i = 0; i < 4; i++) {
        CHECK_THAT(router.routes[i].pattern, Equals(routes[i].pattern));
    }
}

TEST_CASE("Match","[RequestRouter]")
{
    Route routes[] = {
        { GET, "/foo" },
        { POST, "/bar" },
        { GET | POST, "/baz" }
    };
    RequestRouter router(routes, 3 );
    const Route * match;

    HttpRequest request;
    HttpRequest request2;
    HttpRequest request3;

    request.setMethod("POST");
    request.setUrl("/bar");
    match = router.match(request);
    CHECK(match == &(routes[1]));

    request2.setMethod("GET");
    request2.setUrl("/baz");
    match = router.match(request2);
    CHECK(match == &(routes[2]));

    request3.setMethod("POST");
    request3.setUrl("/baz");
    match = router.match(request3);
    CHECK(match == &(routes[2]));
}

TEST_CASE("Not found","[RequestRouter]")
{
    Route routes[] = {
        { GET, "/" }
    };
    RequestRouter router(routes, 1 );
    const Route * match;

    HttpRequest request;
    request.setMethod("GET");
    request.setUrl("/foo");

    match = router.match(request);
    CHECK(match == nullptr);
    CHECK(router.getRouteError() == E_NOT_FOUND);
}

TEST_CASE("Method not allowed", "[RequestRouter]")
{
    Route routes[] = {
        { GET, "/" }
    };
    RequestRouter router(routes, 1);
    const Route * match;

    HttpRequest request;
    request.setMethod("PUT");
    request.setUrl("/");
    
    match = router.match(request);
    CHECK(match == nullptr);
    CHECK(router.getRouteError() == E_METHOD_NOT_ALLOWED);
    CHECK(router.lastAllowedMethods() == GET);
}

TEST_CASE("Url Wildcards","[RequestRouter]")
{

    CHECK(RequestRouter::urlMatches("/foo","/foo") == true);
    CHECK(RequestRouter::urlMatches("/foo/","/foo") == false);
    CHECK(RequestRouter::urlMatches("/foo/","/foo/") == true);
    CHECK(RequestRouter::urlMatches("/*","/foo") == true);
    CHECK(RequestRouter::urlMatches("/*/","/foo") == false);
    CHECK(RequestRouter::urlMatches("/foo/*/bar","/foo/baz/bar") == true);
    CHECK(RequestRouter::urlMatches("*","foo") == true);
    
}
