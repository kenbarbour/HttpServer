#include "catch.hpp"
#include "WebKernel.h"

unsigned int foo;

unsigned int bar;

void do_foo(HttpRequest& request, HttpResponse& response)
{
    foo++;
    response.code = 200;
    response.print("Foo");
}

void do_bar(HttpRequest& request, HttpResponse& response)
{
    bar++;
    response.code = 201;
    response.print("Bar");
}

TEST_CASE("Test WebKernel", "[WebKernel]")
{
    Route routes[] = {
        {GET | POST, "/foo", do_foo},
        {GET | POST, "/bar", do_bar}
    };

    WebKernel kernel(80, routes, sizeof routes / sizeof routes[0]);

    SECTION("No client") {
        kernel.handleClients();
    }

    SECTION("Request for foo") {
        kernel.mock_nextClient("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");
        REQUIRE(foo == 0);
        REQUIRE(bar == 0);
        kernel.handleClients();
        CHECK(foo == 1);
        CHECK(bar == 0);
    }

    SECTION("Request for bar") {
        kernel.mock_nextClient("GET /bar HTTP/1.1\r\nHost: localhost\r\n\r\n");
        REQUIRE(foo == 1);
        REQUIRE(bar == 0);
        kernel.handleClients();
        CHECK(foo == 1);
        CHECK(bar == 1);
    }
}
