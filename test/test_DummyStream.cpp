#include "catch.hpp"
#include "dummy/DummyStream.h"
#include <iostream>

TEST_CASE("DummyStream is availble","[DummyStream]")
{
    const char* input = "Foo\nBar\nBaz\n";
    DummyStream stream(input);
    REQUIRE(strlen(input) > 0);
    REQUIRE(stream.available());
}

TEST_CASE("DummyStream is not available","[DummyStream]")
{
    const char input[1] = {0};
    DummyStream stream(input);
    REQUIRE(strlen(input) == 0);
    CHECK(stream.available()==0);
}


TEST_CASE("DummyStream read","[DummyStream]")
{
    const char* input = "Foo";
    DummyStream stream(input);
    REQUIRE(stream.available() == 3);
    REQUIRE(stream.read() == 'F');
    REQUIRE(stream.available() == 2);
    REQUIRE(stream.read() == 'o');
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'o');
    REQUIRE(stream.available() == 0);
}

TEST_CASE("DummyStream peek","[DummyStream]")
{
    const char* input = "Bar";
    DummyStream stream(input);
    REQUIRE(stream.peek() == 'B');
    REQUIRE(stream.available() == 3);
    REQUIRE(stream.read() == 'B');
    REQUIRE(stream.peek() == 'a');
    REQUIRE(stream.available() == 2);
}

TEST_CASE("DummyStream is a Stream","[DummyStream]")
{
    const char* input = "Baz";
    Stream* stream = new DummyStream(input);
    delete(stream);
}
