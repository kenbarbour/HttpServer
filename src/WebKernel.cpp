#include "WebKernel.h"

void WebKernel::handleClients()
{
    bool shouldYield = false;
    bool keepClient = true;

    if (_state == S_IDLE) {
        _client = _server.available();
        if (!_client) return;

        _request = HttpRequest();
        _parser.reset();
        _state = S_RECEIVING;
        _stateChange = millis();
    } 

    if (millis() - _stateChange >= WEBKERNEL_MAX_WAIT) {
        keepClient = false;
    } else if (_client.connected()) {
        switch (_state) {
            case S_IDLE:
                break;
            case S_RECEIVING:
                if (!_parser.parse()) {
                    shouldYield = true;
                    break;
                }
                _state = S_DISPATCHING;
                _stateChange = millis();
                _client.flush();
            case S_DISPATCHING:
                HttpResponse response(_resp_buffer);
                _dispatcher.handle(_request, response);
                response.headers.set("Connection","close");
                _client.print(response);
                break; 
        }
    } else {
        keepClient = false;
    }

    if (!keepClient) {
        _client.stop();
        _state = S_IDLE;
        _resp_buffer.flush();
    }

    if (shouldYield)
        yield();

}


