#include "Buffer.h"

Buffer::Buffer(uint8_t * ptr, size_t len)
    : start(ptr), size(len), num(0), w_ptr(ptr), r_ptr(ptr)
{}

size_t Buffer::availableToWrite()
{
    return size - num;
}

int Buffer::available()
{
    return num;
}

size_t Buffer::write(uint8_t byte)
{
    if (availableToWrite() == 0) return 0;
    if (w_ptr >= start + size) w_ptr = start;
    *(w_ptr++) = byte;
    num++;
    return 1;
}

size_t Buffer::write(uint8_t * buff, size_t len)
{
    size_t avail = availableToWrite();
    size_t toWrite = (avail < len) ? avail : len;
    for (size_t i = 0; i < toWrite; i++) {
        if (w_ptr >= start + size) w_ptr = start;
        *(w_ptr++) = buff[i];
    }
    num += toWrite;

    return toWrite;
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
