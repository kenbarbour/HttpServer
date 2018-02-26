#include "catch.hpp"
#include "Buffer.h"

TEST_CASE("Buffer Constructor","[Buffer][Constructor]")
{
    uint8_t ptr[3];
    Buffer buffer(ptr, 3);

    CHECK(buffer.available() == 0);
    CHECK(buffer.availableToWrite() == 3);
}

TEST_CASE("Test write byte","[Buffer][write][byte]")
{
    uint8_t ptr[3];
    Buffer buffer(ptr, 3);

    buffer.write(0xFF);
}

TEST_CASE("Test read empty byte","[Buffer][read][byte]")
{
    uint8_t ptr[3];
    Buffer buffer(ptr, 3);

    CHECK(buffer.read() == 0);
}

TEST_CASE("Test read byte","[Buffer][write][read][byte]")
{
    uint8_t ptr[3];
    Buffer buffer(ptr, 3);

    buffer.write(0xFF);
    CHECK(buffer.available() == 1);
    CHECK(buffer.read() == 0xFF);
    CHECK(buffer.available() == 0);
}

TEST_CASE("Test wrap byte","[Buffer][write][read][byte]")
{
    uint8_t ptr[3];
    Buffer buffer(ptr, 3);

    for (int i = 0; i < 10; i++) {
        // buffer is empty
        REQUIRE(buffer.available() == 0);
        REQUIRE(buffer.availableToWrite() == 3);

        // write 1 byte
        buffer.write(i);
        REQUIRE(buffer.available() == 1);
        REQUIRE(buffer.availableToWrite() == 2);

        // write 2nd byte
        buffer.write(i+1);
        REQUIRE(buffer.available() == 2);
        REQUIRE(buffer.availableToWrite() == 1);

        // write 3rd byte (now full)
        buffer.write(i+2);
        REQUIRE(buffer.available() == 3);
        REQUIRE(buffer.availableToWrite() == 0);

        // read bytes in order
        CHECK(buffer.read() == i);
        CHECK(buffer.read() == i+1);
        CHECK(buffer.read() == i+2);

        // write then read 1 more byte (to "randomize" internal pointer location)
        buffer.write(0xFF);
        buffer.read();
    }
}

TEST_CASE("Test wrap string","[Buffer][read][write][string]")
{
    uint8_t ptr[5];
    Buffer buffer(ptr, 5);
    char str[] = "foo";
    char str2[] = "bar";

    REQUIRE(buffer.write((uint8_t *) str, 3) == 3);
    REQUIRE(buffer.available() == 3);
    REQUIRE(buffer.availableToWrite() == 2);
    REQUIRE(buffer.read() == 'f');
    REQUIRE(buffer.read() == 'o');
    REQUIRE(buffer.read() == 'o');
    REQUIRE(buffer.available() == 0);
    REQUIRE(buffer.availableToWrite() == 5);

    REQUIRE(buffer.write((uint8_t *) str2, 3) == 3);
    REQUIRE(buffer.available() == 3);
    REQUIRE(buffer.availableToWrite() == 2);
    REQUIRE(buffer.read() == 'b');
    REQUIRE(buffer.read() == 'a');
    REQUIRE(buffer.read() == 'r');
    REQUIRE(buffer.available() == 0);
    REQUIRE(buffer.availableToWrite() == 5);

}

TEST_CASE("Write string without length","[Buffer][write][string]")
{
    uint8_t ptr[5];
    Buffer buffer(ptr, 5);

    REQUIRE(buffer.write("Foo!") == 4);
}

TEST_CASE("Buffer is a Stream","[Buffer][Stream]")
{
    uint8_t ptr[5];
    Stream * buffer;
    buffer = new Buffer(ptr, 5);

    buffer->write('F');
    REQUIRE(ptr[0] == 'F');
    REQUIRE(buffer->read() == 'F');

    buffer->write("Foo");

}
