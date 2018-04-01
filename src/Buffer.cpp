#include "Buffer.h"

Buffer::Buffer(uint8_t * ptr, size_t len)
    : start(ptr), size(len), num(0), w_ptr(ptr), r_ptr(ptr)
{}

int Buffer::availableForWrite()
{
    return size - num;
}

int Buffer::available() const
{
    return num;
}

int Buffer::available()
{
    return num;
}

size_t Buffer::write(uint8_t byte)
{
    if (availableForWrite() == 0) return 0;
    if (w_ptr >= start + size) w_ptr = start;
    *(w_ptr++) = byte;
    num++;
    return 1;
}

int Buffer::read()
{
    if (available() == 0) return 0;
    if (r_ptr >= start + size) r_ptr = start;
    num--;
    return *(r_ptr++);
}

int Buffer::peek()
{
    if (available() == 0) return 0;
    if (r_ptr >= start + size) r_ptr = start;
    return *(r_ptr);
}

size_t Buffer::printTo(Print& stream) const
{
    size_t av = available();
    size_t written = 0;
    uint8_t * ptr = r_ptr;
    while (written < av) {
        if (ptr >= start + size) ptr = start;
        written += stream.write(*(ptr++));
    }
    return written;
    
}

void Buffer::clear()
{
    w_ptr = start;
    r_ptr = start;
    num = 0;
}
