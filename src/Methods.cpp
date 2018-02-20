#include "Methods.h"

uint8_t method_str(const char * str)
{
    if (strcmp(str, "GET") == 0)
        return GET;
    if (strcmp(str, "POST") == 0)
        return POST;
    if (strcmp(str, "PUT") == 0)
        return PUT;
    if (strcmp(str, "HEAD") == 0)
        return HEAD;
    if (strcmp(str, "DELETE") == 0)
        return DELETE;
    if (strcmp(str, "OPTIONS") == 0)
        return OPTIONS;

    return 0;
}
