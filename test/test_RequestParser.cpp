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
    CHECK_THAT(request.getHttpVersion(), Equals("HTTP/1.1"));

    client.write("Host: localhost\r\n");
    CHECK(parser.parse() == 0);
    CHECK_THAT(request.headers.get("Host"), Equals("localhost"));

    client.write("\r\n");
    CHECK(parser.parse());
    CHECK(!parser.error());
}
TEST_CASE("Test message data","[RequestParser]")
{
    uint8_t _buff[256] = {};
    Buffer client(_buff, 256);
    HttpRequest request;
    RequestParser parser(request, client);

    client.write("POST /foo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 10\r\n\r\n1234567890");
    CHECK(parser.parse());
    CHECK(request.getMessageLength() == 10);
    CHECK_THAT(request.getMessage(), Equals("1234567890"));
}

TEST_CASE("Test parse multiple requests","[RequestParser]")
{
    uint8_t buff[256] = {};
    Buffer client(buff, 256);
    HttpRequest request;
    RequestParser parser(request, client);

    client.write("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");
    CHECK(parser.parse());
    CHECK_THAT(request.getUrl(), Equals("/foo"));
    parser.reset();

    client = Buffer(buff, 256);
    request = HttpRequest();
    client.write("GET /bar HTTP/1.1\r\nHost: localhost\r\n\r\n");
    CHECK(parser.parse());
    CHECK_THAT(request.getUrl(), Equals("/bar"));

}

TEST_CASE("Test parse POST then GET", "[RequestParser][POSTthenGET]")
{
  uint8_t buff[512] = {};
  Buffer client(buff, 512);
  HttpRequest request;
  RequestParser parser(request, client);

  client.write("POST /foo HTTP/1.1\r\nHost: localhost\r\nUser-Agent: curl/7.55.1\r\nAccept: */*\r\nContent-Length: 20\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nssid=foo&passphrase=");
  CHECK(parser.parse());
  CHECK(!client.available());
  CHECK_THAT(request.getUrl(), Equals("/foo"));
  CHECK_THAT(request.getMethod(), Equals("POST"));
  parser.reset();

  client = Buffer(buff, 256);
  request = HttpRequest();
  client.write("GET /foo HTTP/1.1\r\nHost: localhost\r\nUser-Agent: curl/7.55.1\r\nAccept: */*\r\n\r\n");
  CHECK(client.available() == 76);
  CHECK(client.peek() == 'G');
  CHECK(parser.parse());
  CHECK_THAT(request.getUrl(), Equals("/foo"));
  CHECK_THAT(request.getMethod(), Equals("GET"));
}

TEST_CASE("More POST testing", "[RequestParser][POST]")
{
    uint8_t buff[512] = {};
    Buffer client(buff, 512);
    HttpRequest request;
    RequestParser parser(request, client);

    client.write("POST / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.55.1\r\nAccept: */*\r\nContent-Length: 3\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nfoo");
    CHECK(parser.parse());
    CHECK(!client.available());
    CHECK_THAT(request.getUrl(), Equals("/"));
    CHECK_THAT(request.headers.get("Host"), Equals("localhost:8080"));
    CHECK_THAT(request.headers.get("Content-Length"), Equals("3"));

}
