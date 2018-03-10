#include "catch.hpp"
#include "HttpHeaders.h"
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
