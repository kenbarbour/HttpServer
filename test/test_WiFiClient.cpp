#include "catch.hpp"
#include "WiFiClient.h"

TEST_CASE("Test WiFiClient","[WiFiClient]")
{
    WiFiClient client("Foo");
    CHECK(client.available() == 3);
    CHECK(client.availableForWrite() == WIFICLIENT_BUFFER - 3);
    CHECK(client.connected());
    
    client.stop();
    CHECK(!client.connected());
}

TEST_CASE("Test empty WiFiClient","[WiFiClient]")
{
    WiFiClient client;
    CHECK(!client.connected());
    CHECK(client.available() == 0);
}

TEST_CASE("Test bool operator","[WiFiClient]")
{
    WiFiClient client;
    CHECK(!client);

    WiFiClient client2("Foo");
    CHECK(client2);
}
