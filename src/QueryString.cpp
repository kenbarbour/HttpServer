#include "QueryString.h"
#include <string.h>

/**
 * Create a buffer long enough to hold all the string data,
 * parse the query, filling the buffer, and creating the fields
 */
QueryString::QueryString(const char* query) : n(0)
{
  if (query == nullptr || query[0] == '\0') return;

  // create buffer
  buffer_length = urlenc::decoded_length(query) + 2;
  buffer = (char*) malloc(buffer_length);

  // create fields
  n = QueryString::str_count_fields(query);
  fields = (QueryStringField*) malloc(sizeof(QueryStringField) * n);

  size_t i = 0; // index of character in query
  size_t j = 0; // index of character in buffer
  size_t k = 0; // index of field in fields
  char c;

  fields[0].name = buffer;
  fields[0].value = nullptr;
  do {
    c = query[i];
    if (c == '=') {
      c = '\0';
      fields[k].value = buffer + j + 1;
    } else if (c == '&') {
      c = '\0';
      fields[++k].name = buffer + j + 1;
      fields[k].value = nullptr;
    } else if (c == '%') {
      c = urlenc::enctochar(query+i);
      i += 2;
    }
    buffer[j++] = c;
  } while (query[i++] != '\0');
  buffer[j] = '\0';
}

size_t QueryString::count() const
{
  return n;
}

size_t QueryString::count(const char* key) const
{
  size_t count = 0;
  size_t i = 0;
  while (i < n) {
    if (strcmp(fields[i].name, key) == 0) count++;
    i++;
  }
  return count;
}

const char* QueryString::key(const char* name) const
{
  return key(name, 0);
}

const char* QueryString::key(const char* key, size_t n) const
{
  long i = find(key, n);
  if (i == -1) return nullptr;
  return fields[i].value;
}

const char* QueryString::keyAt(size_t index) const
{
  if (index >= n) return nullptr;
  return fields[index].name;
}

const char* QueryString::valueAt(size_t index) const
{
  if (index >= n) return nullptr;
  return fields[index].value;
}

bool QueryString::has(const char* key) const
{
  return (find(key, 0) != -1);
}

long QueryString::find(const char* key, size_t nth) const
{
  size_t matches = 0;
  size_t i = 0;
  while (i < n) {
    if (strcmp(fields[i].name, key) == 0) matches++;
    if (matches > nth) return i;
    i++;
  }
  return -1;
}

size_t QueryString::str_count_fields(const char* query)
{
  if (query[0] == '\0') return 0;
  size_t n = 1;
  size_t i = 0;
  char c;
  do {
    c = query[i];
    n += (c == '&');
    i++;
  } while (c != '\0');

  return n;
}

QueryString::~QueryString()
{
  if (n) {
    free(buffer);
    free(fields);
  }
}
