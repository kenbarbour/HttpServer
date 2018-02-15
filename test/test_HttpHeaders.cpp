#include "catch.hpp"
#include "HttpHeaders.h"
#include "string.h"

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
    CHECK(strcmp(headers.get("foo"),"bar") == 0);
    CHECK(headers.count() == 1);
}

TEST_CASE("Overwrite header with smaller header","[HttpHeaders]"){
    HttpHeaders headers;
    headers.set("foo","sushi");
    headers.set("foo","soy");

    CHECK(strcmp(headers.get("foo"),"soy") == 0);
    CHECK(headers.count() == 1);
}

TEST_CASE("Overwrite header with larger header","[HttpHeaders]"){
    HttpHeaders headers;
    headers.set("foo","pear");
    headers.set("foo","watermelon");
    
    CHECK(strcmp(headers.get("foo"),"watermelon") == 0);
    CHECK(headers.count()==1);
}

TEST_CASE("Force headers to grow","[HttpHeaders]") {
    HttpHeaders headers;
    char * name = "header_";
    /*
    for (int i = 0; i < 30; i++) {
        name[6] = 0x41 + i;
        headers.set(name,"Foo");
        REQUIRE(strcmp(headers.get(name),"Foo") == 0);
    }
    */
}
