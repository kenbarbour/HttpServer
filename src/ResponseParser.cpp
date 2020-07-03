#include "ResponseParser.h"

void ResponseParser::reset() {
    _state = S_IN_HTTPVER;
    _i = _buffer;
}

bool ResponseParser::parse() {
    char in;
    while (server.available() && _state != S_ERROR) {
        switch (_state) {
            case S_IN_HTTPVER:
                in = server.read();
                if (in == ' ') {
                    *(_i++) = '\0';
                    response.setHttpVersion(_buffer);
                    _i = _buffer;
                    _state = S_IN_CODE;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_CODE:
                in = server.read();
                // skip preceding whitespace
                if (in == ' ') {
                    if (_i == _buffer) continue;
                    *(_i) = '\0';
                    response.code = atoi(_buffer);
                    _i = _buffer;
                    _state = S_IN_REASON;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_REASON:
                in = server.read();
                if (in == '\r') continue;
                if (in == '\n') {
                    *(_i) = '\0';
                    response.setReason(_buffer);
                    _i = _buffer;
                    _state = S_IN_HEADER;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_HEADER:
                in = server.read();
                if (in == '\r') continue;
                if (in == '\n') {
                    if (_i == _buffer) {
                        _state = S_IN_MESSAGE;
                        _i = _buffer;
                        break;
                    }
                    *(_i) = '\0';
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
                    response.headers.set(name, value);
                    _i = _buffer;
                    break;
                }
                *(_i++) = in;
                break;
            case S_IN_MESSAGE:
                while (server.available()) {
                    response.content->write(server.read());
                }
                _state = S_COMPLETE;
            case S_COMPLETE:
                return true;
                break;
            default:
                _state = S_ERROR;
        }
    }

    return (_state == S_COMPLETE || _state == S_ERROR);
}