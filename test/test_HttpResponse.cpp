#include "catch.hpp"
#include "HttpResponse.h"

using Catch::Matchers::Equals;

TEST_CASE("Instantiate HttpResponse", "[HttpResponse]")
{
    HttpResponse response;
}

TEST_CASE("Default Response is 200 OK", "[HttpResponse]")
{
    HttpResponse response;
    REQUIRE( response.code == 200 );
    REQUIRE_THAT(response.getReason(), Equals("OK"));
}

TEST_CASE("Set Reason","[HttpResponse]")
{
    HttpResponse response;
    response.setReason("Foo");
    CHECK( response.code == 200 );
    REQUIRE_THAT(response.getReason(), Equals("Foo"));
}

TEST_CASE("Unset reason", "[HttpResponse]")
{
    HttpResponse response;
    response.setReason("Foo");
    REQUIRE(response.code == 200);
    response.setReason(nullptr);
    REQUIRE_THAT(response.getReason(), Equals("OK")); // unsetting reason should restore default
}

TEST_CASE("HttpResponse printTo", "[HttpResponse]")
{
    uint8_t response_buffer[100];
    uint8_t print_buffer[100];
    Buffer message(response_buffer, 100);
    Buffer client(print_buffer, 100);
    HttpResponse r(message);

    r.code = 200;
    r.write("Response Body");
    r.headers.set("Cookie","1234");
    client.print(r);

    CHECK_THAT((char *)print_buffer, Equals("200 OK\r\nCookie: 1234\r\n\r\nResponse Body"));
}
