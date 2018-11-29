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

TEST_CASE("Set url to nullptr", "[HttpRequest]")
{
  HttpRequest req;
  req.setUrl(nullptr);
  bool isnull = (req.getUrl() == nullptr);
  CHECK(isnull);
}

TEST_CASE("Set url multiple times", "[HttpRequest][HttpRequest::setUrl]")
{
  HttpRequest req;
  req.setUrl("/foo");
  req.setUrl("/barbaz");
  CHECK_THAT(req.getUrl(), Equals("/barbaz"));
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

TEST_CASE("HttpRequest empty reassignment", "[HttpRequest][HttpRequest-reassign]")
{
    HttpRequest req;
    req.setUrl("/");
    req.setMethod("GET");

    req = HttpRequest();
    bool isnull = (req.getUrl() == nullptr);
    CHECK(isnull);
}

TEST_CASE("HttpRequest reassignment", "[HttpRequest][HttpRequest-reassign]")
{
    HttpRequest req;
    req.setUrl("/foo/bar");
    req.setMethod("POST");
    req.setMessage("foo=bar&baz=qux");
    req.headers.set("Accept","*/*");

    HttpRequest req2;
    req2.setUrl("/not/foo");
    req2.setMethod("GET");
    req2.headers.set("Content-Type","bar");
    req2 = req;
    CHECK_THAT(req2.getMethod(), Equals("POST"));
    CHECK_THAT(req2.getMessage(), Equals("foo=bar&baz=qux"));
    CHECK_THAT(req2.getUrl(), Equals("/foo/bar"));
    CHECK(req2.headers.has("Accept"));
    CHECK(!req2.headers.has("Content-Type"));
    CHECK_THAT(req2.headers.get("Accept"), Equals("*/*"));
}
