#include "HttpRequest.h"
#include <iostream>

#define HTTPREQUEST_TRIM(stream) while(stream.peek()==' ') { stream.read(); }
#define HTTPREQUEST_BUFFER_UNTIL(buff, strm, stop, n)\
    for (n=0; strm.peek() != stop;n++) \
        { buff[n] = (char) strm.read(); }

void HttpRequest::capture()
{
    long int i, k;
    char buffer[HTTPREQUEST_MAX_MESSAGE_SIZE] = {0};

    HTTPREQUEST_TRIM(client);

    // Read method until ' '
    //for (i=0; client.peek() != ' '; i++) buffer[i] = (char) client.read();
    HTTPREQUEST_BUFFER_UNTIL(buffer, client, ' ', i);
    strncpy(method, buffer, HTTPREQUEST_METHOD_SIZE);
    for (k=0; k <= i; k++) buffer[k] = '\0'; //reset buffer

    HTTPREQUEST_TRIM(client);

    // Read url until ' '
    for (i=0; client.peek() != ' '; i++) buffer[i] = (char) client.read();
    url = (char*) malloc( i + 1 );
    strcpy(url, buffer);
    for (k=0; k <= i; k++) buffer[k] = '\0'; //reset buffer

    HTTPREQUEST_TRIM(client);   
 
    // Read httpver until '\r'
    for (i=0; client.peek() != '\r'; i++) buffer[i] = (char) client.read();
    strncpy(httpver, buffer, HTTPREQUEST_HTTPVER_SIZE);
    for (k=0; k <= i; k++) buffer[k] = '\0'; //reset buffer

    // discard line break
    if (client.peek() == '\r') client.read();
    if (client.peek() == '\n') client.read();

    //read each line, parsing headers, discarding \r\n until a blank line is reached
    while (client.peek() != '\r' && client.peek() != '\n') {
        for (i=0;  client.peek() != ':'; i++) // copy client contents into buffer
            buffer[i] = (char) client.read();
        client.read(); // discard the ':'
        HTTPREQUEST_TRIM(client); // discard whitespace
        buffer[i++] = '\0';
        k = i; // start of value
        for (; client.peek() != '\r'; i++) buffer[i] = (char) client.read();
        headers.set(buffer, (buffer + k) );
        for (k=0; k <= i; k++) buffer[k] = '\0'; //reset buffer
    
        // discard next line break
        if (client.peek() == '\r') client.read();
        if (client.peek() == '\n') client.read();
    }
    client.read();
    if (client.peek() == '\n') client.read(); // consume \r\n

    // Determine or guess the length of the message
    long int content_length;
    if (headers.has("Content-Length") && !headers.has("Transfer-Encoding")) {
        char* length_hdr = headers.get("Content-Length");
        char* length_hdr_end = strlen(length_hdr) + length_hdr;
        content_length = strtol(length_hdr, &length_hdr_end, 10);
    } else {
        content_length = HTTPREQUEST_MAX_MESSAGE_SIZE;
    }

    // copy rest of message
    message = (char*) malloc( content_length + 1);
    if (!message) { 
        //todo handle out of memory
    }
    for (i = 0; i < content_length && client.available(); i++) {
        message[i] = (char) client.read();
    }
    message[i] = '\0';

    // handle content_length too short
    if (client.available()) {
        //todo: content length too short
    }
    message_length = i;
    
}

HttpRequest::~HttpRequest()
{
    if (url) free(url);
    if (message) free(message);
}
