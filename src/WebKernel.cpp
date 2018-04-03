#include "WebKernel.h"

void WebKernel::handleClients()
{
    bool shouldYield = false;
    bool keepClient = true;

    if (_state == S_IDLE) {
        _client = _server.available();
        if (!_client) return;

#ifdef DEBUG
        Serial.println("New client");
#endif
        _request = HttpRequest();
        _parser.reset();
        _state = S_RECEIVING;
        _stateChange = millis();
    } 

    if (millis() - _stateChange >= WEBKERNEL_MAX_WAIT) {
        keepClient = false;
#ifdef DEBUG
        Serial.println("Time limit reached.  Client will be disconnected");
#endif
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
                if (_parser.error()) {
                    keepClient = false;
                    break;
                }
                dispatchRequest();
                keepClient = false;
                break; 
        }
    } else {
        keepClient = false;
    }

    if (!keepClient) {
#ifdef DEBUG
        Serial.println("Closing connection to client");
#endif
        _client.stop();
        _state = S_IDLE;
    }

//    if (shouldYield)
        yield();

}

void WebKernel::dispatchRequest()
{
    HttpResponse response;
    response.setHttpVersion(_request.getHttpVersion());
    _dispatcher.handle(_request, response);
    response.send(_client);
    if (_terminateHandler != nullptr)
        _terminateHandler(_request, response);
}
