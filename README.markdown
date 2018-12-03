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

Included here, a guide for implementing a Web Application as part of your Arduino project.  A WebKernel instance is what handles incoming requests, and hands off control to user defined callback functions. For simplicity, this guide stores variables and implements functions globally.

### Overview
A <tt>WebKernel</tt> object instance provides a method <tt>handleClients()</tt> that should be called in the sketch's <tt>loop</tt> function, and is responsible for parsing incoming HTTP requests and dispatching them to user-defined handlers.

In order to use <tt>WebKernel</tt>. your web application will:
  * Require an array of <tt>Route</tt> that defines which callback handlers get called for each request
  * Require callback functions for each route defined in the routes array
  * Likely need an initialization callback, performing tasks with the <tt>HttpRequest</tt> and <tt>HttpResponse</tt> objects before any routing takes place, such as configuring a buffer or reading cookies
  * Likely need a termination callback, performing tasks with the <tt>HttpRequest</tt> and <tt>HttpResponse</tt> after the response has been sent, such as logging or closing file pointers
  * Potentially benefit from custom handlers for <em>File Not Found</em> or <em>Method Not Allowed</em> errors.

### Defining Routes
A <tt>WebKernel</t> object needs an array of <tt>Route</tt>s in order to map incoming requests to a callback.  A <tt>Route</tt> is a struct containing an 8-bit mask of method flags, a c-stype string pointer to a URL pattern, and a callback function to handle requests matching the route.  The first route that matches the request will be used.

An example Route array:
```
Route routes[] = {
  { GET, "/", index },
  { GET|POST, "/foo", foo}       // GET or POST request will match
  { GET,PUT, "/bar/*/baz", bar}  // URL wildcard
  { GET, "/static/#", static}    // handle anything starting with /static/ ...
};
```
In the above example:
* A GET request for the "/" URL will be handled by the <tt>index(HttpRequest&, HttpResponse&)</tt> callback.
* A request for "/foo" with either GET or POST will be handled by <tt>foo(HttpRequest&, HttpResponse&)</tt>.
* Any GET or PUT request matching the "/bar/\*/baz" pattern ('\*' matches anything but '/') will be handled by <tt>bar(HttpRequest&, HttpResponse&)</tt>.  Multiple wildcards are allowed.
* A GET request for any url starting with "/static/" ... will be handled by <tt>static(HttpRequest&, HttpResponse&)</tt>.  This is useful when content in a certain path is stored entirely on a filesystem.
* All other requests will result in either a 404 (Not Found) or a 405 (Method Not Allowed).

### Web callbacks
Each route defined will have a callback to a <em>void</em> function accepting the arguments: <tt>HttpRequest&</tt> and <tt>HttpResponse&</tt>.  When a request matches the route, these callbacks are responsible for reading the data in the <tt>HttpRequest&</tt>, applying some side-effects for non-GET requests (such as writing to a file or changing some hardware state), and crafting a <tt>HttpResponse&</tt>.

<tt>HttpRequest</tt> provides some useful methods:
* <tt>getMethod()</tt>: gets the HTTP method used in the request
* <tt>getUrl()</tt>: gets the path and query part or the URL
* <tt>getMessage()</tt>: gets the message part of the request for non-GET requests
* <tt>getMessageLength()</tt>: gets the number of bytes in the message

<tt>HttpResponse</tt> provides:
* <tt>int code</tt>: HTTP status code (defaults to 200 OK)
* <tt>setReason(const char*), getReason()</tt>: custom reason code
* <tt>setContent(Stream&)</tt>: sets the content of the message to a <tt>Stream</tt> such as <tt>Buffer</tt> or <tt>File</tt>

### Response Buffer & Init handler
While it is not required, it is recommended to declare a <tt>Buffer</tt> to hold response data in scope, and a <tt>WebKernel</tt> init handler to manage it.  <tt>HTTPResponse.content</tt> represents the content of the message returned to the user agent, and can be any <tt>Stream</tt> object.  For convenience, <tt>Buffer</tt> is a <tt>Stream</tt> that allows response content to be built using the <tt>print</tt> functions.  The init handler can clear the buffer with each new request.

```
// Outside of function definitions, so the Buffer isn't destroyed
uint8_t _buffer_data[512];    // 512 bytes of buffer
Buffer content(_buffer_data, sizeof(_buffer_data));

void init_handler(HttpRequest& request, HttpResponse& response) {
  response.content = content;
  content.clear();
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

### Serving Files
Since response content is a <tt>Stream</tt>, <tt>File</tt> objects can be used instead of a <tt>Buffer</tt>.  In order to not fall out of scope once the handler finishes, the <tt>File</tt> should be declared outside of the route handler, and use the terminate handler to close the file.  A simple way to do this:

```
// Routes and WebKernel defined above

uint8_t _buffer_data[512];
Buffer content(_buffer_data, sizeof(_buffer_data)); // Buffer defined as usual

File contentFile;     // File declared

void kernel_terminate(const HttpRequest& request, const HttpResponse& response) {
  if (contentFile)  // If file is open, close it
    contentFile.close();
}

void setup() {
  webKernel.begin();
  webKernel.setTerminateHandler(kernel_terminate);
  // ...
}

```
### Drive
The <tt>WebKernel</tt> has everything it needs to handle requests now.  The only thing left is to make a call to <tt>webKernel.handleClients()</tt> as part of your sketch's <tt>loop()</tt> function.

```
void loop()
{
	webKernel.handleClients();
	// ... include other sketch routines
}
```

## Examples

### Minimal Example
Here is a minimal example, without any WiFi or other arduino features, of a basic implementation with a single function that handles GET requests for any URL.

```
#include <WebKernel.h>

// Content Buffer
uint8_t _buffer_data[100];
Buffer content(_buffer_data, sizeof(_buffer_data)); // content buffer

// Initialize WebKernel
void init_webkernel(HttpRequest& req, HttpResponse& res) {
  content.clear();
  res.content = content;
}

// Handle Requests
void handleURL(HttpRequest& req, HttpResponse& res) {
  res.headers.set("Content-Type","text/plain");
  content.printf("It works! The current url is %s", req.getUrl());
}

// Define WebKernel and Routes
Route routes[] = {
  {GET, "/#", handleURL}
};
size_t numRoutes = sizeof(routes) / sizeof(Route);
WebKernel webKernel(80, routes, numRoutes);  // Run webKernel on port 80 with routes array

void setup() {
  webKernel.begin();
  webKernel.setInitHandler(init_webkernel);
}

void loop() {
  webKernel.handleClients();
}

```

### Basic Web Application
Open the basic example in your Arduino IDE and upload it to your ESP8266.  These examples use the ESP8266 as a Standalone Access Point, so refer to the sketch for the SSID and password, or connect it to a Serial monitor before it starts.

To test, connect to its wireless access point and go to (http://192.168.1.4/), the default IP address for the ESP.

### Filesystem Web Application
Similar to the basic example, except this example also includes static content in the _data_ directory that needs to be uploaded separately.

To upload the static content to an ESP8266, you will need to use the ESP8266FS tool described here: (https://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#uploading-files-to-file-system)
