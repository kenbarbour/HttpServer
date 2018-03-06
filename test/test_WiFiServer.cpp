#include "catch.hpp"
#include "WiFiServer.h"

TEST_CASE("Test next","[WiFiServer]")
{
    WiFiServer server(80);
    CHECK(server.port == 80);
}
