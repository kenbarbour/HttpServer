#include "RequestParser.h"

bool RequestParser::parse() {

    while (client.available() && _state != S_ERROR) {
        char in = client.read();

        switch (_state) {
            case S_IN_METHOD:
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
                if (in == '\r') continue;
                if (in == '\n') {
                    if (_i == _buffer) {
                        if (request.headers.has("Content-Length"))
                            _state = S_IN_MESSAGE;
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
            case S_COMPLETE:
                return true;
            default:
                _state = S_ERROR;

        }

    }

    return (_state == S_COMPLETE || _state == S_ERROR);
}
