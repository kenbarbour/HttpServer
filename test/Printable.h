#pragma once

#include <stdlib.h>

class Print;

class Printable {
    public:
        virtual size_t printTo(Print& p) const = 0;
};
