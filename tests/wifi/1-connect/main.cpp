#include "Arduino.h"
#include "WiFi.h"
int lostConnectionCount = 0;
 

/** Callback for receiving IP address from AP */
void gotIP(system_event_id_t event) {
  Serial.println("Online. Local IP address:");
  Serial.println(WiFi.localIP().toString());
  // We are connected, we could already receive or send something w/WiFi
}
 
/** Callback for connection loss */
void lostCon(system_event_id_t event) {
  ++lostConnectionCount;
  Serial.printf("WiFi lost connection try %d to connect again\n", lostConnectionCount);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (lostConnectionCount==4) {
    Serial.println("Cannot connect to the internet. Check your WiFI credentials");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup() started let's connect to WiFI");
 
  Serial.printf("WiFi name: %s\nWiFi pass: %s\n", WIFI_SSID, WIFI_PASS);
// Start our connection attempt
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 

// Event driven WiFi callbacks
// Setup callback function for successful connection
//  WiFi.onEvent(gotIP, SYSTEM_EVENT_STA_GOT_IP);
 
 // Note: this onEvent handler used to work for ESP32, now it seems is not there anymore
// Setup callback function for lost connection
//  WiFi.onEvent(lostCon, SYSTEM_EVENT_STA_DISCONNECTED);
 
}
 
void loop() {
}