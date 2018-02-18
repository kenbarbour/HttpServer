#pragma once

typedef struct requestHandler {
    HttpRequest request;
    char** slugs;
    respond();
} RequestHandler;

class RequestRouter
{
    public:
        char* match(Request);
};
