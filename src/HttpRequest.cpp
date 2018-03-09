#include "HttpRequest.h"
#include "string.h"

HttpRequest::HttpRequest(): method(), url(), httpver(), message(), message_length(0) {}

const char * HttpRequest::setMethod(const char * method)
{
    strncpy(this->method, method, HTTPREQUEST_METHOD_SIZE);
    return this->method;
}

const char * HttpRequest::getMethod()
{
    return method;
}

const char * HttpRequest::setHttpVer(const char * ver)
{
    strncpy(this->httpver, ver, HTTPREQUEST_HTTPVER_SIZE);
    return this->httpver;
}

const char * HttpRequest::getHttpVer()
{
    return httpver;
}

const char * HttpRequest::setUrl(const char * url, size_t n)
{
    if (this->url) free(this->url);
    this->url =  (char *) malloc(n + 1);
    strncpy(this->url, url, n+1);
    return this->url;
}

const char * HttpRequest::setUrl(const char * url)
{
    size_t len = strlen(url);
    return this->setUrl(url, len);
}

const char * HttpRequest::getUrl()
{
    return this->url;
}

const char * HttpRequest::setMessage(const char * message)
{
    size_t len = strlen(message);
    return this->setMessage(message, len);
}

const char * HttpRequest::setMessage(const char * message, unsigned int n)
{
    if (this->message) free(this->message);
    this->message = (char *) malloc(n + 1);
    message_length = n;
    strncpy(this->message, message, n+1);
    return this->message;
}

const char * HttpRequest::getMessage()
{
    return this->message;
}

HttpRequest::~HttpRequest()
{
    if (url) {
        free(url);
        url = nullptr;
    }
    if (message) {
        free(message);
        url = nullptr;
    }
}
