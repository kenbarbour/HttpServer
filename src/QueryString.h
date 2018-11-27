#ifndef _QUERYSTRING_H
#define _QUERYSTRING_H

#include <stdlib.h>
#include "urlenc.h"

typedef struct QueryStringField {
  char * name;
  char * value;
} QueryStringField;

class QueryString {
public:
  QueryString(const char* );
  ~QueryString();

  size_t count() const;
  size_t count(const char*) const;

  const char* key(const char*) const;
  const char* key(const char*, size_t) const;
  const char* keyAt(size_t) const;
  const char* valueAt(size_t) const;
  bool has(const char*) const;

  static size_t str_count_fields(const char*);

private:
  char* buffer;
  size_t buffer_length;
  QueryStringField* fields;
  size_t n;
  long find(const char*, size_t) const;
};

#endif // _QUERYSTRING_H include guard
