#include "catch.hpp"
#include "HttpRequest.h"
#include <iostream>

using Catch::Matchers::Equals;

TEST_CASE("Create and capture an HTTP Request","[HttpRequest]") {
    const char * req_str = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    DummyStream client(req_str);
    HttpRequest req(client);
    req.capture();

    CHECK_THAT(req.method, Equals("GET"));
    CHECK_THAT(req.url, Equals("/"));
    CHECK_THAT(req.httpver, Equals("HTTP/1.1"));

    REQUIRE(req.headers.has("Host"));
    CHECK_THAT(req.headers.get("Host"), Equals("localhost"));

}

TEST_CASE("Capture a complicated HTTP Request","[HttpRequest]") {
    const char * req_str = "POST /foo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 11\r\n\r\nfoo=bar&baz";
    DummyStream client(req_str);
    HttpRequest req(client);
    req.capture();

    CHECK_THAT(req.url, Equals("/foo"));
    CHECK_THAT(req.headers.get("Content-Length"), Equals("11"));
    CHECK_THAT(req.message, Equals("foo=bar&baz"));
}

