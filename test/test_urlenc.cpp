#include "catch.hpp"
#include "urlenc.h"

#include <iostream>

using namespace urlenc;
using Catch::Matchers::Equals;

TEST_CASE("Convert some encodings to characters","[urlenc][enctochar]")
{
  CHECK(enctochar("%20") == ' ');
  CHECK(enctochar("%21") == '!');
  CHECK(enctochar("%2B") == '+');
}

TEST_CASE("Stop decoding after 3 characters","[urlenc][enctochar]")
{
  CHECK(enctochar("%20foobarbaz") == ' ');
}

TEST_CASE("Replace all the %__ encodings with the proper character","[urlenc][decode]")
{
  char buff[20] = {};
  char lil[4] = {};

  CHECK(decode("foo=bar%20baz", buff, 12) == 12);
  CHECK_THAT(buff, Equals("foo=bar baz"));

  // Long string
  CHECK(decode("AAAABBBBCCCCDDDDEEEEFFFF", buff, 12) == 12);
  CHECK_THAT(buff, Equals("AAAABBBBCCC"));

  // Long string with encodings
  CHECK(decode("AAAA%20BBBB%20CCCC", buff, 12) == 12);
  CHECK_THAT(buff, Equals("AAAA BBBB C"));

  // This would be a malformed string, but this is my expected behavior
  CHECK(decode("hack%", buff, 12) == 6);
  CHECK_THAT(buff, Equals("hack%"));
  CHECK(decode("hack%r", buff, 12) == 7);
  CHECK_THAT(buff, Equals("hack%r"));

  // Another overrun Test
  CHECK(decode("AA%", buff, 4) == 4);
  CHECK_THAT(buff, Equals("AA%"));
}

TEST_CASE("Test the hexval helper function","[urlenc][hexval]")
{
  CHECK(hexval('0') == 0);
  CHECK(hexval('1') == 1);
  CHECK(hexval('2') == 2);
  CHECK(hexval('3') == 3);
  CHECK(hexval('4') == 4);
  CHECK(hexval('5') == 5);
  CHECK(hexval('6') == 6);
  CHECK(hexval('7') == 7);
  CHECK(hexval('8') == 8);
  CHECK(hexval('9') == 9);
  CHECK(hexval('A') == 10);
  CHECK(hexval('B') == 11);
  CHECK(hexval('C') == 12);
  CHECK(hexval('D') == 13);
  CHECK(hexval('E') == 14);
  CHECK(hexval('F') == 15);
  CHECK(hexval('a') == 10);
  CHECK(hexval('b') == 11);
  CHECK(hexval('c') == 12);
  CHECK(hexval('d') == 13);
  CHECK(hexval('e') == 14);
  CHECK(hexval('f') == 15);

  // undocumented behavior if char is not a hex
}

TEST_CASE("Check the length of a decoded string","[urlenc][decoded_length]")
{
  CHECK(decoded_length("foo%20bar%21baz%22") == 12);
  CHECK(decoded_length("%20bar%21baz%22") == 9);
  CHECK(decoded_length("bla?bla=bla&blabla") == 18);
  CHECK(decoded_length("foo%") == 4);
  CHECK(decoded_length("AAA%A") == 5);
}

TEST_CASE("Locate position of a key/value pair in the query component","[urlenc][query_key_pos]")
{
  CHECK(query_key_pos("foo=1&bar=2&baz=3","bar") == 6);
  CHECK(query_key_pos("foo=1&bar=2&baz=3","foo") == 0);
  CHECK(query_key_pos("foo=1&bar=2&baz=3","qux") == -1);
  CHECK(query_key_pos("foo=1&bar=&baz=3","baz") == 11);
  CHECK(query_key_pos("j=10&bar=&baz=3","bar") == 5);
  CHECK(query_key_pos("","") == -1);
  CHECK(query_key_pos("","bar") == -1);
  CHECK(query_key_pos("foo=bar&baz=qux","") == -1);
}
