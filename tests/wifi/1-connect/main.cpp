#include "Arduino.h"
#include "WiFi.h"
int lostConnectionCount = 0;
 

void setup() {
  Serial.begin(115200);
  Serial.println("setup() started let's connect to WiFI");
 
  Serial.printf("WiFi name: %s\nWiFi pass: %s\n", WIFI_SSID, WIFI_PASS);
// Start our connection attempt
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
// Event driven WiFi callbacks are different check the mdns example
}
 
void loop() {
}