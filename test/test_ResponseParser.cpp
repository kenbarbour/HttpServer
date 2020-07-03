#include "catch.hpp"
#include "ResponseParser.h"
#include <HttpResponse.h>
#include <Buffer.h>

using Catch::Matchers::Equals;

TEST_CASE("Test parsing a response with headers and content")
{
    uint8_t _reqbuff[100];
    Buffer server(_reqbuff, 100);
    uint8_t _resbuff[100];
    Buffer buffer(_resbuff, 100);
    HttpResponse response;
    ResponseParser parser(response, server);
    
    response.content = &buffer;

    server.print("HTTP/1.0 200 OK\r\nCookie: foo=bar; baz=qux;\r\nContent-Length:10\r\n\r\nABCDEFGHIJ");

    bool result = parser.parse();

    CHECK_THAT(response.getHttpVersion(), Equals("HTTP/1.0"));
    CHECK(response.code == 200);
    CHECK_THAT(response.getReason(), Equals("OK"));

    CHECK(response.headers.count() == 2);
    CHECK(response.headers.has("Cookie") == true);
    CHECK_THAT(response.headers.get("Cookie"), Equals("foo=bar; baz=qux;"));
    CHECK(response.headers.has("Content-Length") == true);
    CHECK_THAT(response.headers.get("Content-Length"), Equals("10"));
    CHECK_THAT((char*)_resbuff, Equals("ABCDEFGHIJ"));

}