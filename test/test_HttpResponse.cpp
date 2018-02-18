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
