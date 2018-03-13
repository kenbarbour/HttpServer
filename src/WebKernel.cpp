#include "WebKernel.h"

void WebKernel::handleClients()
{
    bool shouldYield = true;
    bool keepClient = false;

    if (_state == S_IDLE) {
        WiFiClient client = _server.available();
        if (!client) return;

        _client = client;
        _request = HttpRequest();
        _state = S_RECEIVING;
        _stateChange = millis();
    } 

    if (_client.connected()) {
        switch (_state) {
            case S_IDLE:
                break;
            case S_RECEIVING:
                if (!_parser.parse()) {
                    if (millis() - _stateChange <= WEBKERNEL_MAX_WAIT)
                        keepClient = false;
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

    if (millis() - _stateChange <= WEBKERNEL_MAX_WAIT) keepClient = false;

    if (!keepClient) {
        _client.stop();
        _state = S_IDLE;
        _resp_buffer.flush();
    }

    if (shouldYield)
        yield();

}


