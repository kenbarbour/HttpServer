#include "catch.hpp"
#include "WiFiClient.h"

TEST_CASE("Test WiFiClient","[WiFiClient]")
{
    WiFiClient client;
    CHECK(client.available() == 0);
    CHECK(client.availableForWrite() == WIFICLIENT_BUFFER);
    CHECK(client.connected());
    
    client.stop();
    CHECK(!client.connected());
}
