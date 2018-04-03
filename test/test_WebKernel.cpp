#include "catch.hpp"
#include "WebKernel.h"

unsigned int foo;

unsigned int bar;

char error_message[24] = {};

char response_buffer[100] = {};
Buffer message((uint8_t*)response_buffer, 100);
bool term = false;
bool init = false;

void do_foo(HttpRequest& request, HttpResponse& response)
{
    foo++;
    response.code = 200;
    response.content = &message;
    message.clear();
    message.print("Foo");
}

void do_bar(HttpRequest& request, HttpResponse& response)
{
    bar++;
    response.code = 201;
    response.content = &message;
    message.clear();
    message.print("Bar");
}

void handle_notFound(HttpRequest& request, HttpResponse& response)
{
    response.code = 454; // unusual code
    strcpy(error_message, "File Not Found");
}

void handle_methodNotAllowed(HttpRequest& request, HttpResponse& response)
{
    response.code = 455; //unusual code
    strcpy(error_message, "Method Not Allowed");
}

void handle_terminate(const HttpRequest& request, const HttpResponse& response)
{
    term = true;
}

void handle_init(HttpRequest& request, HttpResponse& response)
{
    init = true;
}

TEST_CASE("Test WebKernel", "[WebKernel]")
{
    Route routes[] = {
        {GET | POST, "/foo", do_foo},
        {GET | POST, "/bar", do_bar}
    };

    WebKernel kernel(80, routes, sizeof routes / sizeof routes[0]);

    foo = 0;
    bar = 0;

    SECTION("No client") {
        kernel.handleClients();
    }

    SECTION("Request for foo") {
        kernel.mock_nextClient("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");
        REQUIRE(foo == 0);
        REQUIRE(bar == 0);
        kernel.handleClients();
        CHECK(foo == 1);
        CHECK(bar == 0);
    }

    SECTION("Request for bar") {
        kernel.mock_nextClient("GET /bar HTTP/1.1\r\nHost: localhost\r\n\r\n");
        REQUIRE(foo == 0);
        REQUIRE(bar == 0);
        kernel.handleClients();
        CHECK(foo == 0);
        CHECK(bar == 1);
    }

}

TEST_CASE("Test Partial requests", "[WebKernel]")
{
    Route routes[] = {
        { GET, "/foo", do_foo },
        { GET, "/bar", do_bar }
    };
    WebKernel kernel(80, routes, 2);
    foo = bar = 0;

    SECTION("Partial GET") {
        kernel.mock_nextClient("GET /foo");
        kernel.handleClients();
        REQUIRE(foo == 0);
        REQUIRE(bar == 0);
        WiFiClient& client = kernel.mock_currentClient();
        CHECK(client.available() == 0);
        delay(100);
        client.write(" HTTP/1.1\r\nHost:localhost\r\n\r\n");
        kernel.handleClients();
        CHECK(foo == 1);
        CHECK(client.available() > 0);
    }

    SECTION("Timeout") {
        REQUIRE(foo == 0);
        kernel.mock_nextClient("GET /foo");
        kernel.handleClients();
        WiFiClient& client = kernel.mock_currentClient();
        kernel.mock_nextClient("GET /bar HTTP/1.1\r\nHost: localhost\r\n\r\n");
        delay(WEBKERNEL_MAX_WAIT);
        kernel.handleClients();  //
        REQUIRE(foo == 0);
        REQUIRE(bar == 0);
        CHECK(client.connected() == false);
        kernel.handleClients();
        REQUIRE(foo == 0);
        REQUIRE(bar == 1);
    }
}

TEST_CASE("Test WebKernel error handling", "[WebKernel]")
{

}

TEST_CASE("WebKernel init handler", "[WebKernel]")
{
    Route routes[] = {
        { GET, "/foo", do_foo },
        { GET, "/bar", do_bar }
    };
    WebKernel kernel(80, routes, 2);
    foo = bar = 0;
    term = false;
    init = false;
    kernel.setInitHandler(handle_init);
    kernel.mock_nextClient("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");

    kernel.handleClients();

    CHECK(init == true);
}

TEST_CASE("WebKernel terminate handler", "[WebKernel]")
{
    Route routes[] = {
        { GET, "/foo", do_foo },
        { GET, "/bar", do_bar }
    };
    WebKernel kernel(80, routes, 2);
    foo = bar = 0;
    term = false;

    kernel.mock_nextClient("GET /foo HTTP/1.1\r\nHost: localhost\r\n\r\n");
    kernel.setTerminateHandler(handle_terminate);

    kernel.handleClients();
    CHECK(term == true);
}
