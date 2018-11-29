#include "HttpRequest.h"
#include "string.h"

HttpRequest::HttpRequest(): method(), url(), httpver(), message(), message_length(0) {}

HttpRequest& HttpRequest::operator=(const HttpRequest& other)
{
  if (&other == this) return *this;
  this->setMethod(other.getMethod());
  this->setUrl(other.getUrl());
  this->setHttpVersion(other.getHttpVersion());
  this->setMessage(other.getMessage());
  memcpy(&this->headers, &other.headers, sizeof(HttpHeaders));
  return *this;
}

const char * HttpRequest::setMethod(const char * method)
{
    strncpy(this->method, method, HTTPREQUEST_METHOD_SIZE);
    return this->method;
}

const char * HttpRequest::getMethod() const
{
    return method;
}

const char * HttpRequest::setHttpVersion(const char * ver)
{
    strncpy(this->httpver, ver, HTTPREQUEST_HTTPVER_SIZE);
    return this->httpver;
}

const char * HttpRequest::getHttpVersion() const
{
    return httpver;
}

const char * HttpRequest::setUrl(const char * url, size_t n)
{
    if (this->url != nullptr) free(this->url);
    this->url =  (char *) malloc(n + 1);
    strncpy(this->url, url, n+1);
    return this->url;
}

const char * HttpRequest::setUrl(const char * url)
{
  if (url == nullptr) {
    if (this->url != nullptr) free(this->url);
    this->url = nullptr;
    return this->url;
  }
  size_t len = strlen(url);
  return this->setUrl(url, len);
}

const char * HttpRequest::getUrl() const
{
    return this->url;
}

const char * HttpRequest::setMessage(const char * str)
{
    if (str == nullptr) {
      this->message = nullptr;
      message_length = 0;
      return this->message;
    }
    size_t len = strlen(str);
    return this->setMessage(str, len);
}

const char * HttpRequest::setMessage(const char * message, unsigned int n)
{
    if (this->message) free(this->message);
    this->message = (char *) malloc(n + 1);
    message_length = n;
    strncpy(this->message, message, n+1);
    return this->message;
}

const char * HttpRequest::getMessage() const
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
