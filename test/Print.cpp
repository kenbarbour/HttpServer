#include "Print.h"

size_t Print::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--) {
        if (write(*buffer++)) n++;
        else break;
    }
    return n;
}

size_t Print::print(int i)
{
    size_t len = 0;
    if (i < 0) len += print('-');
    i = (unsigned int) i << 1;
    i = (unsigned int) i >> 1;
    len += printNumber(i, 10);

    return len;
}

size_t Print::print(const Printable& x)
{
    return x.printTo(*this);
}

size_t Print::println(void)
{
    return write("\r\n");
}

size_t Print::println(const char* str)
{
    size_t n = print(str);
    n += println();
    return n;
}

size_t Print::println(char c)
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(int i)
{
    size_t n = print(i);
    n += println();
    return n;
}

size_t Print::println(const Printable& x)
{
    size_t n = x.printTo(*this);
    n += println();
    return n;
}

size_t Print::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}
