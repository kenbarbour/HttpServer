#pragma once
#include <stdint.h>
#include <string.h>

#define GET (1 << 0)
#define HEAD (1 << 1)
#define POST (1 << 2)
#define PUT (1 << 3)
#define DELETE (1 << 4)
#define OPTIONS (1 << 5)

uint8_t method_str(const char * str);

