	
#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;
// A very simple one liner PHP that just returns the time for this TZ
String url = "http://fs.fasani.de/time.php?tz=Europe/Berlin";
 

#define LED_GPIO 17   // Not important

int lostConnectionCount = 0;

#ifdef S2
void lostCon(arduino_event_id_t event) {
  ++lostConnectionCount;
  Serial.printf("WiFi lost connection try %d to connect again\n", lostConnectionCount);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (lostConnectionCount==4) {
    Serial.println("Cannot connect to the internet. Check your WiFI credentials");
    delay(2000);
    ESP.restart();
  }
}

void online(){
  digitalWrite(LED_GPIO, 1);
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());

  http.begin(url);
  int httpCode = http.GET();  //Make the request
  Serial.printf("http status:%d\n", httpCode);
 
  if (httpCode == 200) { 
     String response = http.getString();
     Serial.printf("The time now is: %s\n", response);
  } else {
    Serial.printf("We could not read the time.\nPlease check in the browser that the given url: %s is correct and replies with an HTTP 200 OK status", url);
  }
}

void gotIP(arduino_event_id_t event) {
    online(); 
}
#else  // Then this is an ESP32
  void gotIP() {
    online();
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
// Sorry for the ugly cast, at the end is of course cleaner to use the constants that have the right type
#ifdef S2
  WiFi.onEvent(gotIP, (arduino_event_id_t)7);
  WiFi.onEvent(lostCon, (arduino_event_id_t)5);
#else
  // No idea why trying to use the onEvent's w/ESP32 didn't work, was not being called, let's do it old way ;)
  Serial.print("Connecting.");
  uint8_t counter=0;
  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(200);
    ++counter;
    // Note at least with my router many times it stays in the loop and does not connect, restarting solves the issue
    if (counter>20) {
      ESP.restart();
    }
  }
  gotIP();
#endif
}

void loop() {}