#include "catch.hpp"
#include "RouteDispatcher.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Buffer.h"
#include "DummyStream.h"

void index(HttpRequest& request, HttpResponse& response)
{
//    response.message->write("This is the index");
}

uint8_t responseData[256];
Buffer responseBuffer(responseData, 256);
Route routes[] = {
    { GET, "/", index },
};
RequestRouter router(routes, sizeof routes / sizeof routes[1]);
RouteDispatcher dispatcher(router);

TEST_CASE("Kernel-like device","[Kernel]")
{
    Buffer responseBuffer((uint8_t *) responseData, 256);
    DummyStream client("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    HttpRequest request(client);
    HttpResponse response;
    response.message = &responseBuffer;
}

