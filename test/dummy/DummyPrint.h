#ifndef DUMMYPRINT_H
#define DUMMYPRINT_H

class Print
{
    public:
        Print() {}
        virtual size_t write(uint8_t) = 0;
        virtual size_t write(const uint8_t* buffer, size_t size) {
            size_t written;
                for (int i = 0; i < size; i++)
                    written += write(buffer[i]);
            return written;
        }

        size_t write(const char * buffer, size_t size) {
            return write((const uint8_t *)buffer, size);
        }
        virtual void flush() {}
};

class DummyPrint: public Print
{
    public:
        DummyPrint(char * buff): buffer(buff), pos(0) {};
        virtual size_t write(uint8_t c) { buffer[pos++]=(char) c; buffer[pos]='\0';return 1; }
        int pos;
        char * buffer;
};


#endif // DUMMYPRINT_H
