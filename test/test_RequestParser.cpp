#include "catch.hpp"
#include "Buffer.h"
#include "RequestParser.h"
using Catch::Matchers::Equals;

TEST_CASE("Test GET","[RequestParser]")
{
    uint8_t _buff[256] = {};
    Buffer client(_buff, 256);
    HttpRequest request;
    RequestParser parser(request, client);

    client.write("GET ");
    CHECK(parser.parse() == 0);
    CHECK_THAT(request.getMethod(), Equals("GET"));

    client.write("/foo ");
    CHECK(parser.parse() == 0);
    CHECK_THAT(request.getUrl(), Equals("/foo"));

    client.write("HTTP/1.1\r\n");
    CHECK(parser.parse() == 0);
    CHECK_THAT(request.getHttpVer(), Equals("HTTP/1.1"));

    client.write("Host: localhost\r\n");
    CHECK(parser.parse() == 0);
    CHECK_THAT(request.headers.get("Host"), Equals("localhost"));

    client.write("\r\n");
    CHECK(parser.parse());
    CHECK(!parser.error());
}
