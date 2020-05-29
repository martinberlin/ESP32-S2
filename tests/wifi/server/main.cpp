	
#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>  // multicast DNS
#include <WebServer.h>
#define LED_GPIO 18

int lostConnectionCount = 0;
char apName[] = "espressif";
bool switchState = false;
WebServer server(80); // port where we start the server
 
// Callback see defineServerRouting
void onServerSwitch(){
   switchState = !switchState;
   digitalWrite(LED_GPIO, switchState);
   String switchStatus = "OFF";
   if (switchState) {
     switchStatus = "ON";
   }
   server.send(200, "text/html", switchStatus);
}
 
void onServerNotFound(){
  Serial.println("404");
  server.send(200, "text/html", "404 this route is not defined.\nPlease check in the browser that the given url: %d is correct and defineServerRouting()");
}
 
// ROUTING Definitions. Add your server routes here
void defineServerRouting() {
  server.on("/switch", HTTP_GET, onServerSwitch);
  server.onNotFound(onServerNotFound);
}
 
void online(){
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());

    MDNS.begin(apName);
    MDNS.addService("http", "tcp", 80);
    Serial.println(String(apName)+".local mDns started");
    defineServerRouting();
    server.begin();

    Serial.println("Server started");
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup() started let's connect to WiFI");
 
  Serial.printf("WiFi name: %s\nWiFi pass: %s\n", WIFI_SSID, WIFI_PASS);
// Start our connection attempt
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(200);
  } 
  online();

}
 
void loop() {
  // Run our server engine
  server.handleClient();
}