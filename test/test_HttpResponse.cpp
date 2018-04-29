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

TEST_CASE("HttpResponse send", "[HttpResponse]")
{
    uint8_t response_buffer[100] = "AAAABBBBCCCCDDDDEEEE";
    uint8_t print_buffer[100];
    Buffer message(response_buffer, 100);
    Buffer client(print_buffer, 100);
    HttpResponse r(message);
    REQUIRE(message.available() == 0);

    r.setHttpVersion("HTTP/1.1");

    r.code = 200;
    message.write("AAAABBBBCCCCDDDD");
    r.headers.set("Cookie","1234");
    r.content = &message;
    r.send(client);

    CHECK_THAT((char *)print_buffer, Equals("HTTP/1.1 200 OK\r\nCookie: 1234\r\n\r\nAAAABBBBCCCCDDDD"));
}

TEST_CASE("HttpResponse get/setHttpVersion", "[HttpResponse]")
{
    HttpResponse r;

    SECTION("Empty") {
        r.setHttpVersion("");
        CHECK_THAT(r.getHttpVersion(), Equals(""));
    }

    SECTION("HTTP/1.1") {
        r.setHttpVersion("HTTP/1.1");
        CHECK_THAT(r.getHttpVersion(), Equals("HTTP/1.1"));
    }

    SECTION("Overflow buffer") {
        r.setHttpVersion("AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPP");
    }
}

TEST_CASE("HttpResponse setContent", "[HttpResponse]")
{
    HttpResponse r;
    uint8_t buff[10];
    Buffer content(buff, sizeof(buff));

    r.setContent(content);
}
