#include "catch.hpp"
#include "RouteDispatcher.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Buffer.h"
#include "DummyStream.h"

#include <iostream>

using Catch::Matchers::Equals;

void index(HttpRequest& request, HttpResponse& response)
{
    response.write("Site Index");
}

void save(HttpRequest& request, HttpResponse& response)
{
    response.write("You sent post data");
    response.code = 201;
}

void create(HttpRequest& request, HttpResponse& response)
{
    response.write("Foo form");
}

uint8_t requestData[256];
uint8_t responseData[256];
Route routes[] = {
    { GET, "/", index },
    { POST, "/foo", save },
    { GET, "/foo", create }
};
RequestRouter router(routes, sizeof routes / sizeof routes[1]);
RouteDispatcher dispatcher(router);

TEST_CASE("Test index","[Kernel]")
{
    Buffer requestBuffer(requestData, 256);
    Buffer responseBuffer(responseData, 256);
    requestBuffer.write("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    
    HttpRequest request(requestBuffer);
    HttpResponse response(responseBuffer);
    
    dispatcher.handle(request, response);
    
    CHECK_THAT((const char *)responseData, Equals("Site Index"));
    CHECK(response.code == 200);
    CHECK_THAT(response.getReason(), Equals("OK"));
}

TEST_CASE("Test post","[Kernel]")
{
    Buffer requestBuffer(requestData, 256);
    Buffer responseBuffer(responseData, 256);
    requestBuffer.write("POST /foo HTTP/1.1\r\nHost: localhost\r\n\r\nfoo=bar");

    HttpRequest request(requestBuffer);
    HttpResponse response(responseBuffer);

    dispatcher.handle(request, response);
    
    CHECK_THAT((const char *)responseData, Equals("You sent post data"));
    CHECK(response.code == 201);
    CHECK_THAT(response.getReason(), Equals("Created"));
}
