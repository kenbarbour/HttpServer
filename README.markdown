HttpServer
==========
<em>A Complicated Arduino Web Server Implementation</em>

## Features
* Designed around the popular ESP8266 playform
* Pattern-based request routing allows a callback to handle URL requests matching simple patterns
* Basic Buffer class provided for Response content, as well as support for File and other Stream-based content
* Simple request lifecycle control allows processing before and after requests are made
* Error handling callback functions

## Usage

Included here, a guide for implementing a Web Application as part of your Arduino project.  For simplicity,
this guide stores variables and implements function globally.

### Define
Include <tt>WebKernel.h</tt> and define a _Routes_ array, _WebKernel_ object, and a _Response Buffer_ to store
response data.  The _WebKernel_ contructor needs the _port_ number, _routes_ array, and the _number_ of routes in the array.

```
/*
 *  File: example.ino
 */
#include <WebKernel.h>

// Define maximum size of content buffer
#define CONTENT_SIZE 512

// TODO: put function prototypes here

Routes routes = {
		{ GET, "/", handle_index },
		{ GET|POST, "/foo", handle_foo },
		{ POST, "/bar", handle_post_bar },
		{ GET, "/bar", handle_get_bar }
	};

WebKernel webKernel(80, routes, sizeof(routes)/sizeof(routes[0]));

uint8_t content_data[CONTENT_SIZE];			// Global block of memory to store content
Buffer content(content_data, CONTENT_SIZE);   // Buffer to write data to previously allocated content_data

```

### Implement
Each route defined in the routes array will need a function implemented to handle requests for that route.
You can put them in the main sketch .ino file anywhere as long as you include a function prototype before the Routes definition.
Alternatively, you could write prototypes in a file like <tt>routes.h</tt>, and implement them in a file like <tt>routes.cpp</tt>.

```
// Function prototypes
void handle_index    (HttpRequest&, HttpResponse&); // handle GET request for '/'
void handle_foo      (HttpRequest&, HttpResponse&); // handle GET or POST request for '/foo'
void handle_post_bar (HttpRequest&, HttpResponse&); // handle POST request for '/bar'
void handle_get_bar  (HttpRequest&, HttpResponse&); // handle GET request for '/bar'
```

Function prototypes should be put before any other reference to one of the functions is made.  In this example, they should be defined before defining the _Route_ array.

Next, implement each of the functions we just defined:

```
void handle_index(HttpRequest& request, HttpResponse& response)
{
	response.setContent(content);
	content.println("<!doctype html><html><head><title>Site Index</title></head><body><h1>Site Index</h1></body></html>");
}
```

### Initialize
Initialize the webKernel in your sketch's <tt>setup</tt> function.

```
void setup()
{
	// initialize Serial, WiFi, Ethernet, etc

	webKernel.begin();
}
```

### Drive
The WebKernel has everything it needs to handle requests now.  The only thing left is to make a call to <tt>webKernel.handleClients()</tt> as part of your sketch's <tt>loop</tt> function.

```
void loop()
{

	webKernel.handleClients();

	// ... include other sketch routines
}
```

### Additional Callbacks
For more complicated projects, such as projects also serving static files or other Stream objects, its helpful to have more control over the WebKernel throughout the request handling process.  It is also helpful to have control over _Method Not Allowed_ and _File Not Found_ errors.

Each of the callbacks listed below are of the form `void handler (HttpRequest&, HttpResponse&)` with the exception of the Terminate Handler, which is `void terminate_handler (const HttpRequest&, const HttpResponse&)`.

```
void setup()
{
	// ... other setup routines

	webKernel.begin();
	webKernel.setInitHandler(init_webkernel); // called before dispatching to the routes handler
	webKernel.setTerminateHandler(term_webkernel); // called after sending response to client
	webKernel.setNotFoundHandler(handle_not_found); // called if a route is not found for the request
	webKernel.setMethodNotAllowedHandler(handle_method_not_allowed); // called if a route is found, but the request method is not allowed

}
```

## Examples

### Basic Web Application
Open the basic example in your Arduino IDE and upload it to your ESP8266.  These examples use the ESP8266 as a Standalone Access Point, so refer to the sketch for the SSID and password, or connect it to a Serial monitor before it starts.

To test, connect to its wireless access point and go to (http://192.168.1.4/), the default IP address for the ESP.

### Filesystem Web Application
Similar to the basic example, except this example also includes static content in the _data_ directory that needs to be uploaded separately.

To upload the static content to an ESP8266, you will need to use the ESP8266FS tool described here: (https://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#uploading-files-to-file-system)

