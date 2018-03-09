#include "catch.hpp"
#include "WebKernel.h"

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

