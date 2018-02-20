#include "RequestRouter.h"
#include "catch.hpp"
#include "dummy/DummyStream.h"
#include "NotFoundException.h"
#include "MethodNotAllowedException.h"
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

    DummyStream client("POST /bar HTTP/1.1\r\nHost: localhost\r\n\r\nfoobar");
    DummyStream client2("GET /baz HTTP/1.1\r\nHost: localhost\r\n\r\nfoobar");
    DummyStream client3("POST /baz HTTP/1.1\r\nHost: localhost\r\n\r\nbaaz");    

    HttpRequest request(client);
    HttpRequest request2(client2);
    HttpRequest request3(client3);

    match = router.match(request);
    CHECK(match == &(routes[1]));

    match = router.match(request2);
    CHECK(match == &(routes[2]));

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

    DummyStream client("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");
    HttpRequest request(client);

    REQUIRE_THROWS_AS(match = router.match(request), NotFoundException);
}

TEST_CASE("Method not allowed", "[RequestRouter]")
{
    Route routes[] = {
        { GET, "/" }
    };
    RequestRouter router(routes, 1);
    const Route * match;

    DummyStream client("PUT / HTTP/1.1\r\nHost: localhost\r\n\r\nfoo");
    HttpRequest request(client);

    REQUIRE_THROWS_AS(match = router.match(request), MethodNotAllowedException);
    
    try {
        router.match(request);
    } catch (MethodNotAllowedException &e) {
        CHECK(e.methodsAllowed == GET);    
    }
}
