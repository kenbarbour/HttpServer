#include "HttpResponse.h"

/**
 * TODO: this is the only constructor really needed unless _TEST_
 */
HttpResponse::HttpResponse(Buffer & buffer): 
    buffer(&buffer), 
    code(200), 
    reason(), 
    httpver() 
{};

HttpResponse::HttpResponse(Buffer & buffer, unsigned int code): 
    buffer(&buffer), 
    code(code), 
    reason(),
    httpver()
{};

HttpResponse::HttpResponse(Buffer & buffer, unsigned int code, const char * reason):
    buffer(&buffer), 
    code(code), 
    reason(),
    httpver()
{
    setReason(reason);
}

HttpResponse::HttpResponse(): 
    buffer(), 
    code(200), 
    reason(),
    httpver() 
{};

HttpResponse::HttpResponse(unsigned int code): 
    buffer(), 
    code(code), 
    reason(),
    httpver()
{}

HttpResponse::HttpResponse(unsigned int code, const char * reason): 
    buffer(), 
    code(code), 
    reason(),
    httpver()
{
    setReason(reason);
}

const char * HttpResponse::setReason(const char * reason)
{
    if (this->reason != nullptr) free(this->reason);
    if (reason == nullptr) return this->reason = nullptr;
    this->reason = (char *) malloc(strlen(reason));
    strcpy(this->reason, reason);

    return this->reason;
}

const char * HttpResponse::getReason() const
{
    if (this->reason) return this->reason;
    return HttpResponse::getDefaultReason(code);
}

const char * HttpResponse::setHttpVersion(const char * version)
{
    strncpy(httpver, version, HTTPRESPONSE_HTTPVER_SIZE - 1);
    httpver[HTTPRESPONSE_HTTPVER_SIZE - 1] = '\0';
    return httpver;
}

const char * HttpResponse::getHttpVersion() const
{
    return httpver;
}

const char * HttpResponse::getDefaultReason(unsigned int code)
{
    switch(code) {
        case 100: return "Continue";
        case 101: return "Switching Protocols";

        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";

        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 307: return "Temporary Redirect";

        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Time-Out";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Large";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested range not satisfiable";
        case 417: return "Expectation Failed";

        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";

        default: return "";
    }
}

size_t HttpResponse::write(uint8_t data)
{
    return buffer->write(data);
}

size_t HttpResponse::write(uint8_t * data, size_t len)
{
    return buffer->write(data, len);
}

size_t HttpResponse::printTo(Print& client) const
{
    size_t len = 0;
    len += client.print(getHttpVersion());
    len += client.print(' ');
    len += client.print((int) code);
    len += client.print(' ');
    len += client.println(getReason());
    len += client.println(headers);
    for (size_t i = buffer->available(); i > 0; i--)
        len += client.print((char)buffer->read());
    return len;
}

HttpResponse::~HttpResponse()
{
    if (reason) free(reason);
}
