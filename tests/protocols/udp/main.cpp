	
#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>  // multicast DNS
#include <AsyncUDP.h>
#include <SimpleTimer.h>

#define LED_GPIO 16   // Not important
#define UDP_PORT 1234 // UDP will hear in this port

int lostConnectionCount = 0;
char apName[] = "espressif";

// Message transport protocol
AsyncUDP udp;
SimpleTimer timer;
// Let's measure the framerate in 1 second snapshots
uint8_t timerEachSec = 1;
uint32_t frameCounter = 0;
uint32_t frameLastCounter = 0;
uint32_t frameTimerCalls = 0;
bool outputUdpPacketsInSerial = false;

void timerCallback(){
  if (frameLastCounter != frameCounter) {
    frameTimerCalls++;
    Serial.printf("FPS %lu / %lu / AVG %lu\n", (frameCounter-frameLastCounter)/timerEachSec, 
	  frameCounter, frameCounter/(frameTimerCalls*timerEachSec));

    frameLastCounter = frameCounter;
  } 
}

void startUdpServer(){
    // Interval to measure FPS  (millis, function called, times invoked for 1000ms around 1 hr and half)
  	timer.setTimer(timerEachSec*1000, timerCallback, 6000);

    if(udp.listen(UDP_PORT)) {
      Serial.println("UDP Listening on: ");
      Serial.print(WiFi.localIP());Serial.println(":"+String(UDP_PORT)); 
	  
    // Executes on UDP receive and dumps the received packet in Serial
    udp.onPacket([](AsyncUDPPacket packet) {
     ++frameCounter;
		 uint16_t receivedLength = packet.length();
     
     if (outputUdpPacketsInSerial){
      for (uint16_t i = 0; i<=receivedLength; i++){
					Serial.printf("%x ", packet.data()[i]);
				}
        Serial.println();
        } else {
          // Output only first byte
          Serial.printf("U%x ",packet.data()[0]);
        }
        if (frameCounter % 100 == 0) {
          Serial.printf("Received %d frames\n", frameCounter);
        }
    }); 
 
    } else {
      Serial.println("UDP Listener could not be started");
    }
}

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
void gotIP(arduino_event_id_t event) {
    digitalWrite(LED_GPIO, 1);
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());
    MDNS.begin(apName);
    MDNS.addService("http", "tcp", 80);
    Serial.println(String(apName)+".local mDns started");
    startUdpServer();
}
#else  // Then this is an ESP32
  void gotIP() {
    Serial.println("Online. Local IP address:");
    Serial.println(WiFi.localIP().toString());
    // We are connected, we could already receive or send something w/WiFi
    startUdpServer();
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

void loop() {
  timer.run();
}