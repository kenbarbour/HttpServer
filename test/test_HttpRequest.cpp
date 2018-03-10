#include "catch.hpp"
#include "HttpRequest.h"
#include <iostream>
#include "DummyStream.h"

using Catch::Matchers::Equals;

TEST_CASE("Create and capture an HTTP Request","[HttpRequest]") {
    const char * req_str = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    DummyStream client(req_str);
    HttpRequest req(client);

    CHECK_THAT(req.method, Equals("GET"));
    CHECK_THAT(req.url, Equals("/"));
    CHECK_THAT(req.httpver, Equals("HTTP/1.1"));

    REQUIRE(req.headers.has("Host"));
    CHECK_THAT(req.headers.get("Host"), Equals("localhost"));

}

TEST_CASE("Capture a complicated HTTP Request","[HttpRequest]") {
    const char * req_str = "POST /foo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 11\r\n\r\nfoo=bar&baz";
    DummyStream client(req_str);
    HttpRequest req;
    req.capture(client);

    CHECK_THAT(req.url, Equals("/foo"));
    CHECK_THAT(req.headers.get("Content-Length"), Equals("11"));
    CHECK_THAT(req.message, Equals("foo=bar&baz"));
}

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
    CHECK(req.message_length == 9);
}

TEST_CASE("HttpRequest reassignment", "[HttpRequest]")
{
    HttpRequest req;
    req.setUrl("/");
    req.setMethod("GET");

    req = HttpRequest();
    CHECK(req.getUrl() == nullptr);
}
