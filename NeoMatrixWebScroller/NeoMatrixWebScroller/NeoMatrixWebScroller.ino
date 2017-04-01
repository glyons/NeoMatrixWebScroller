// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN D2

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *SERVER_WIFI_SSID =  "YOUR WIFI SSID";
const char *SERVER_WIFI_PASS = "YOUR PASSWORD";

void setupWiFi()
{
   Serial.print("Connecting to WiFi ");
   WiFi.begin(SERVER_WIFI_SSID,SERVER_WIFI_PASS);
   while(WiFi.status() != WL_CONNECTED)
   {
     delay(500);
     Serial.print(".");
   }
   Serial.println("Connected");
}


// Instantiate server class                               // See tab Network for #include
ESP8266WebServer server(80);                              // HTTP server will listen at port 80



// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

String textToScroll = "";

void setup() {
  Serial.begin(115200);                                   // full speed to monitor
  setupWiFi();
  webserver();
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}

void loop() {
 server.handleClient();                                  // checks for incoming messages
 display();
}

int x    = matrix.width();
int pass = 0;

void display()
{

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(textToScroll);
  int n = (textToScroll.length()+1)*8;
  if(--x < -n) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
}


String form =                                             // String form to sent to the client-browser
  "<p>"
  "<center>"
  "<h1>Text to scroll</h1>"
  "<form action='msg'><p>Type something nice... <input type='text' name='msg' size=50 autofocus> <input type='submit' value='Submit'></form>"
  "</center>";

void webserver()
{
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []()
  {
    server.send(200, "text/html", form);
  });

  server.on("/msg", handle_msg);                          // And as regular external functions:
  server.begin();                                         // Start the server

  char result[16];
  sprintf(result, "%3d.%3d.%3d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);

  Serial.println("WebServer ready:   ");
  textToScroll = result;
  Serial.println(WiFi.localIP());                         // Serial monitor prints localIP
  Serial.println(); 
  Serial.println(result);
}


