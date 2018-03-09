#include "catch.hpp"
#include "HttpRequest.h"
#include <iostream>
#include "DummyStream.h"

using Catch::Matchers::Equals;
    

TEST_CASE("Test get/set method","[HttpRequest]")
{
    HttpRequest req;
    req.setMethod("POST");
    CHECK_THAT(req.getMethod(), Equals("POST"));
}

TEST_CASE("Test get/set url","[HttpRequest]")
{
    HttpRequest req;
    req.setUrl("/foo");
    CHECK_THAT(req.getUrl(), Equals("/foo"));
}

TEST_CASE("Test get/set short url", "[HttpRequest]")
{
    HttpRequest req;
    req.setUrl("/");
    CHECK(strcmp(req.getUrl(),"/") == 0);
}

TEST_CASE("Test get/set message","[HttpRequest]")
{
    HttpRequest req;
    req.setMessage("FooBarBaz");
    CHECK_THAT(req.getMessage(), Equals("FooBarBaz"));
    CHECK(req.getMessageLength() == 9);
}

TEST_CASE("HttpRequest reassignment", "[HttpRequest]")
{
    HttpRequest req;
    req.setUrl("/");
    req.setMethod("GET");

    req = HttpRequest();
    CHECK(req.getUrl() == nullptr);
}
