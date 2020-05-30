	
#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>  // multicast DNS
#define LED_GPIO 18

int lostConnectionCount = 0;
char apName[] = "espressif";
 
 
#ifdef S2
void lostCon(arduino_event_id_t event) {
  ++lostConnectionCount;
  Serial.printf("WiFi lost connection try %d to connect again\n", lostConnectionCount);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (lostConnectionCount==4) {
    Serial.println("Cannot connect to the internet. Check your WiFI credentials");
  }
}

void gotIP(arduino_event_id_t event) {
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());
    MDNS.begin(apName);
    MDNS.addService("http", "tcp", 80);
    Serial.println(String(apName)+".local mDns started");
}
#else
  // Then this is an ESP32
  void gotIP(system_event_id_t event) {
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());
    // We are connected, we could already receive or send something w/WiFi
  }

  void lostCon(system_event_id_t event) {
    ++lostConnectionCount;
    Serial.printf("WiFi lost connection try %d to connect again\n", lostConnectionCount);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if (lostConnectionCount==4) {
      Serial.println("Cannot connect to the internet. Check your WiFI credentials");
    }
  }
#endif

void setup() {
  Serial.begin(115200);
  Serial.println("setup() started let's connect to WiFI");
 
  Serial.printf("WiFi name: %s\nWiFi pass: %s\n", WIFI_SSID, WIFI_PASS);
// Start our connection attempt
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 

// Event driven WiFi callbacks for ESP32S2
// Let's use the event ID's so it works on both SoCs
// ARDUINO_EVENT_WIFI_STA_GOT_IP=7
// ARDUINO_EVENT_WIFI_STA_DISCONNECTED=5
// Sorry for the ugly cast!
#ifdef S2
  WiFi.onEvent(gotIP, (arduino_event_id_t)7);
  WiFi.onEvent(lostCon, (arduino_event_id_t)5);
  #else
  WiFi.onEvent(gotIP, (system_event_id_t)7);
  WiFi.onEvent(lostCon, (system_event_id_t)5);
#endif
}
 
void loop() {}