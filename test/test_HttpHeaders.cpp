#include "catch.hpp"
#include "HttpHeaders.h"
#include "Buffer.h"
#include "string.h"

using Catch::Matchers::Equals;

TEST_CASE("Empty HttpHeaders","[HttpHeaders]") {
   HttpHeaders headers;

   CHECK(headers.count() == 0);
}

TEST_CASE("Get and set headers","[HttpHeaders]"){
    HttpHeaders headers;

    // Check that getting non-existant header is a nullptr
    CHECK(headers.get("foo") == nullptr);

    // Set 'foo: bar' and check it
    headers.set("foo","bar");
    CHECK_THAT(headers.get("foo"), Equals("bar"));
    CHECK(headers.count() == 1);
}

TEST_CASE("Name contains another name", "[HttpHeaders]")
{
  HttpHeaders h;
  h.set("Accept-Encoding","foo, bar");
  CHECK(h.has("Accept") == false);
}

TEST_CASE("Set headers again","[HttpHeaders]"){
    HttpHeaders headers;
    headers.set("foo","sushi");
    headers.set("foo","soy");

    CHECK_THAT(headers.get("foo"), Equals("soy"));
    CHECK(headers.count() == 1);
}

TEST_CASE("Append headers","[HttpHeaders]")
{
    HttpHeaders headers;
    headers.set("foo","sushi");
    headers.append("foo","soy");

    CHECK_THAT(headers.get("foo"), Equals("sushi, soy"));
    CHECK(headers.count() == 1);
}

TEST_CASE("Append empty header behaves like set","[HttpHeaders]")
{
    HttpHeaders headers;
    headers.append("foo","bar");

    CHECK_THAT(headers.get("foo"), Equals("bar"));
    CHECK(headers.count() == 1);
}

TEST_CASE("Fill headers","[HttpHeaders]") {
    HttpHeaders headers;
    char name[HTTPHEADERS_NAME_SZ] = "";
    strcpy(name, "header");

    for (int i = 0; i < HTTPHEADERS_NUM; i++) {
        name[6] = 0x41 + i;
        headers.set(name,"Foo");
        REQUIRE_THAT(headers.get(name), Equals("Foo"));
        CHECK(headers.count() == (i+1) );
    }
}

TEST_CASE("Header name is case insensitive","[HttpHeaders]") {
    HttpHeaders headers;

    headers.set("Foo","bar");

    CHECK(headers.has("foo"));
}

TEST_CASE("Copy constructor", "[HttpHeaders]")
{
    HttpHeaders orig;
    orig.set("Foo","bar");
    orig.append("Foo","baz");
    orig.set("Bar","qux");
    REQUIRE(orig.count() == 2);

    HttpHeaders copy(orig);
    CHECK(copy.has("Foo"));
    CHECK_THAT(copy.get("Foo"), Equals("bar, baz"));
    CHECK_THAT(copy.get("Bar"), Equals("qux"));
    CHECK(copy.count() == 2);

    orig.set("Bux","foo");
    CHECK(!copy.has("Bux"));
    CHECK(copy.count() == 2);
}

TEST_CASE("HttpHeaders printTo", "[HttpHeaders]")
{
    HttpHeaders h;
    uint8_t buff[100];
    Buffer buffer(buff, 100);

    buffer.print(h);
    CHECK_THAT((char *) buff, Equals(""));
    buffer.flush();

    h.set("Foo","Bar");
    h.set("Bar","baz");
    buffer.print(h);
    CHECK_THAT((char *) buff, Equals("Foo: Bar\r\nBar: baz\r\n"));
}

TEST_CASE("HttpHeaders find in list", "[HttpHeaders][HttpHeaders::in]")
{
  HttpHeaders h;
  h.set("Accept-Encoding","gzip, deflate");
  CHECK(h.in("Accept-Encoding","gzip") == true);
  CHECK(h.in("Accept-Encoding","deflate") == true);
  CHECK(h.in("Accept-Encoding","identity") == false);

  CHECK(h.in("Accept","gzip") == false); // Accept is not in the headers
}

TEST_CASE("Find matches when a q-factor is provided", "[HttpHeaders]")
{
  HttpHeaders h;
  h.set("Accept","text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8");
  CHECK(h.in("Accept","text/html"));
  CHECK(h.in("Accept","application/xhtml+xml"));
  CHECK(h.in("Accept","application/xml"));
  CHECK(h.in("Accept","*/*"));
}
