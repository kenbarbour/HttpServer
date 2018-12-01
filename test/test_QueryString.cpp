#include "catch.hpp"
#include "QueryString.h"
#include "string.h"

using Catch::Matchers::Equals;

TEST_CASE("Construct", "[QueryString]")
{
  const char* query = "foo=bar&baz=qux";
  QueryString qs(query);

  CHECK(qs.count() == 2);
}

TEST_CASE("Count keys", "[QueryString]")
{
  QueryString qs("foo=bar&baz=qux");
  CHECK(qs.count("foo") == 1);
  CHECK(qs.count("baz") == 1);
}

TEST_CASE("Encoded characters", "[QueryString]")
{
  QueryString qs("Foo%20Bar=Baz%3DQux&apple=Orange%26Apple");
  CHECK(qs.count() == 2);
  CHECK_THAT(qs.keyAt(0), Equals("Foo Bar"));
  CHECK_THAT(qs.valueAt(0), Equals("Baz=Qux"));
  CHECK_THAT(qs.keyAt(1), Equals("apple"));
  CHECK_THAT(qs.valueAt(1), Equals("Orange&Apple"));
}

TEST_CASE("Find keys", "[QueryString]")
{
  QueryString qs("foo=bar&baz=qux");
  CHECK_THAT(qs.key("foo"), Equals("bar"));
  CHECK_THAT(qs.key("baz"), Equals("qux"));
  CHECK_THAT(qs.keyAt(0), Equals("foo"));
  CHECK_THAT(qs.valueAt(0), Equals("bar"));
  CHECK_THAT(qs.keyAt(1), Equals("baz"));
  CHECK_THAT(qs.valueAt(1), Equals("qux"));
}

TEST_CASE("Repeated Keys", "[QueryString]")
{
  QueryString qs("foo=1234&foo=5678&foo=bar");
  CHECK(qs.count() == 3);
  CHECK(qs.count("foo") == 3);
  CHECK_THAT(qs.key("foo", 0), Equals("1234"));
  CHECK_THAT(qs.key("foo", 1), Equals("5678"));
  CHECK_THAT(qs.key("foo", 2), Equals("bar"));
}

TEST_CASE("Null values", "[QueryString]")
{
  QueryString qs("foo&bar&baz");
  CHECK(qs.count() == 3);
  CHECK_THAT(qs.keyAt(0), Equals("foo"));
  CHECK_THAT(qs.keyAt(1), Equals("bar"));
  CHECK_THAT(qs.keyAt(2), Equals("baz"));
  CHECK(qs.valueAt(0) == nullptr);
  CHECK(qs.valueAt(1) == nullptr);
  CHECK(qs.valueAt(2) == nullptr);
}

TEST_CASE("Null Empty", "[QueryString]")
{
  QueryString qs("foo=&bar=&baz=");
  CHECK(qs.count() == 3);
  CHECK_THAT(qs.keyAt(0), Equals("foo"));
  CHECK_THAT(qs.keyAt(1), Equals("bar"));
  CHECK_THAT(qs.keyAt(2), Equals("baz"));
  CHECK_THAT(qs.valueAt(0), Equals(""));
  CHECK_THAT(qs.valueAt(1), Equals(""));
  CHECK_THAT(qs.valueAt(2), Equals(""));
}

TEST_CASE("Has", "[QueryString]")
{
  QueryString qs("foo=&bar&baz=qux");
  CHECK(qs.has("foo"));
  CHECK(qs.has("bar"));
  CHECK(qs.has("baz"));
  CHECK(!qs.has("qux"));
}

TEST_CASE("str_count_fields", "[QueryString][str_count_fields]")
{
  CHECK(QueryString::str_count_fields("") == 0);
  CHECK(QueryString::str_count_fields("foo") == 1);
  CHECK(QueryString::str_count_fields("foo&bar") == 2);
  CHECK(QueryString::str_count_fields("foo=bar&baz=qux") == 2);
  CHECK(QueryString::str_count_fields("foo&bar&foo&bar") == 4);
}

TEST_CASE("empty string", "[QueryString][QueryString-empty]")
{
  QueryString qs("");
  CHECK(qs.count() == 0);
}

TEST_CASE("constructor with nullptr", "[QueryString-nullptr]")
{
  QueryString qs(nullptr);
  CHECK(qs.count() == 0);
}
