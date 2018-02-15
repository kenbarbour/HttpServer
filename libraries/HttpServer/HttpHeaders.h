#pragma once

typedef struct HttpHeaderNode
{
  char* name;
  char* value;
} HttpHeaderNode;

class HttpHeaders {
    public:
        HttpHeaders();
        ~HttpHeaders();
        void set(const char * , const char *);
        char* get(const char*);
        bool has(const char*);
        int count();
        int length();
        void write(char *, int);

    private:
        int _n;
        HttpHeaderNode* headers;
};

