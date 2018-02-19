#include "RequestRouter.h"
#include "catch.hpp"
#include "dummy/DummyStream.h"
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
    RequestRouter router(routes, sizeof routes);
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
    RequestRouter router(routes, sizeof routes);
    const Route * match;
    DummyStream client("POST /bar HTTP/1.1\r\nHost: localhost\r\n\r\nfoobar");
    HttpRequest request(client);

    match = router.match(request);
    CHECK(match == &(routes[1]));
}
