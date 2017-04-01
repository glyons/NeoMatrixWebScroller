#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
/*
  handles the messages coming from the webbrowser, restores a few special characters and
  constructs the strings that can be sent to the oled display
*/

void handle_msg() 
{
  server.send(200, "text/html", form);                    // Send same page so they can send another msg

  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  String decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");
  decodedMsg.replace("%21", "!");
  decodedMsg.replace("%22", "");
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");
  decodedMsg.replace("%2C", ",");
  decodedMsg.replace("%2F", "/");
  decodedMsg.replace("%3A", ":");
  decodedMsg.replace("%3B", ";");
  decodedMsg.replace("%3C", "<");
  decodedMsg.replace("%3D", "=");
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");
  decodedMsg.replace("%40", "@");
  //Serial.println(decodedMsg);                           // print original string to monitor
  unsigned int lengte = decodedMsg.length();              // length of received message
  for  (int i = 0; i < lengte; i++)                       // prints up to 8 rows of 16 characters.
  {
    char c = decodedMsg[i];
    Serial.print(c);                                      //decodedMsg[i]);
  }

textToScroll = decodedMsg;
}

