/**
 *  Basic WebKernel Web Application Example
 *    In this example, the ESP8266 acts as a standalone access point and allows
 *  the user to control the builtin LED pin over the web.
 *
 *    To see this example working:
 *    1. Open the Serial Monitor
 *    2. Upload to the ESP8266
 *    3. Note the WiFi SSID, password, and IP Address in the serial monitor
 *    4. Connect to the newly created WiFi Access Point
 *    5. Using the web browser or curl, control the LED pin with the following
 *         URLs: (assuming the default configuration)
 *
 *       * Show the LED status:  http://192.168.4.1/led
 *       * Toggle the LED;       http://192.168.4.1/led/toggle
 */
#include <ESP8266WiFi.h>
#include <WebKernel.h>

const char* network_ssid = "ESPThing";		// Name of the WiFi Access Point to create
const char* network_password = "";		// Password for WiFi (empty is no password)

const uint8_t ledPin = LED_BUILTIN;		// Arduino Pin controlling the LED

uint8_t content_buffer[64] = {};		// global buffer to contain response data
Buffer content(content_buffer, 			// print response data to this variable
		sizeof(content_buffer)/sizeof(content_buffer[0]));


/**
 * Handle requests for the site index (URL: /)
 */
void index(HttpRequest& request, HttpResponse& response)
{
  response.headers.set("Content-Type","text/plain");
  content.println("Site Index");
}

/**
 * Handle requests to show the LED status
 * This function should print the status of the LED pin to content
 */
void show_led(HttpRequest& request, HttpResponse& response)
{
  response.headers.set("Content-Type","text/plain");
  if (digitalRead(ledPin))
    content.println("off");
  else content.println("on");
}

/**
 * Handle requests to toggle the LED state
 * Afterwards, send the user the LED's status
 */
void toggle_led(HttpRequest& request, HttpResponse& response)
{
  digitalWrite(ledPin, !(digitalRead(ledPin)));
  show_led(request, response);
}

/**
 *    Before any request is handled, clear the content buffer from the last
 *  request and setup the buffer for the new request.
 */
void init_webkernel(HttpRequest& request, HttpResponse& response)
{
  content.clear();  // clear previously printed content
  response.content = &content;  // attach content buffer to the response
}

/**
 * Setup a WiFi connection
 * Called during setup routine
 */
void setup_wifi()
{
  // Setup WiFi - Use the ESP as a standalone access point 
  WiFi.mode(WIFI_AP);
  WiFi.softAP(network_ssid, network_password);
  Serial.println("WiFi SSID: ");
  Serial.print(network_ssid);
  Serial.print("WiFi password: ");
  Serial.println(network_password); 
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
}

/**
 *  Define an array of Routes for the WebKernel
 *  Each route contains:
 *   * one or more methods (ex: (GET | POST) handles both GET and POST requests),
 *   * a URL pattern (* matches anything until a slash '/', # matches until the end)
 *   * the name of a function accepting arguments: HttpRequest&, HttpResponse&
 */
Route routes[] = {
  {GET, "/", index},
  {GET, "/led", show_led},
  {GET, "/led/toggle", toggle_led}
};

WebKernel webKernel(80, routes, sizeof(routes)/sizeof(routes[0]));  // Global WebKernel

void setup() {

  // Initialize Pins
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Setup wifi
  setup_wifi();

  // Initialize webKernel
  webKernel.begin();
  webKernel.setInitHandler(init_webkernel); // important! tells the webkernel how to initialize each connection

}

void loop() {
  webKernel.handleClients();
}

