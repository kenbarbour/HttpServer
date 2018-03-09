#include "RequestParser.h"

bool RequestParser::parse() {

    unsigned int content_length = 0, available = 0, writable = 0;
    char in;

    while (client.available() && _state != S_ERROR) {

        switch (_state) {
            case S_IN_METHOD:
                in = client.read();
                if (in == ' ') {
                    *(_i++) = '\0';
                    request.setMethod(_buffer);
                    _i = _buffer;
                    _state = S_IN_URL;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_URL:
                in = client.read();
                // skip preceding whitespace
                if (in == ' ') {
                    if (_i == _buffer) continue;
                    *(_i) = '\0';
                    request.setUrl(_buffer);
                    _i = _buffer;
                    _state = S_IN_HTTPVER;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_HTTPVER:
                in = client.read();
                if (in == '\r') continue;
                if (in == '\n') {
                    *(_i) = '\0';
                    request.setHttpVer(_buffer);
                    _i = _buffer;
                    _state = S_IN_HEADER;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_HEADER:
                in = client.read();
                if (in == '\r') continue;
                if (in == '\n') {
                    if (_i == _buffer) {
                        if (request.headers.has("Content-Length")) {
                            content_length  = atoi(request.headers.get("Content-Length"));
                            _state = S_IN_MESSAGE;
                            _i = _buffer;
                        }
                        else _state = S_COMPLETE;
                        break;
                    }
                    char * name = _buffer;
                    char * value = _buffer;
                    bool s = 0;
                    while (value < _i) {
                        if (s) {
                            if (*value == ' ') value++;
                            else break;
                        } else {
                            if (*value == ':') {
                                s = true;
                                *(value++) = '\0';
                            } else value++;
                        }
                    }
                    request.headers.set(name, value);
                    _i = _buffer;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_MESSAGE:
                available = client.available();
                writable = content_length - (_i - _buffer);
                while (writable && available) {
                    *(_i++) = client.read();
                    writable--;
                    available--;
                }
                if (!writable) {
                    if (available) client.flush();
                    *(_i++) = '\0';
                    request.setMessage(_buffer, content_length);
                    _state = S_COMPLETE;
                    break;
                }
                break;
            case S_COMPLETE:
                return true;
                break;
            default:
                _state = S_ERROR;

        }

    }

    return (_state == S_COMPLETE || _state == S_ERROR);
}
