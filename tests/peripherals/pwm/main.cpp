// Since someone asked about a modern AnalogWrite PWM example
// Just try to compile this one
#include "Arduino.h"
 
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0 0
// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT 13
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ 5000
// Note to see this working you need to atach a LED to the GPIO
// S2 has a ws2812 neopixel, to see example in IDF: https://github.com/espressif/esp-idf/blob/master/examples/peripherals/rmt/led_strip/main/led_strip_main.c
#define LED_PIN 17

bool brightDirection = 0;
int ledBright = 0;
 
void setup() {
  Serial.begin(115200);
  Serial.println("Hello PWM LED");
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
}
// Arduino like analogWrite: value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}
 
void loop() {
  // set the brightness on LEDC channel 0
  ledcAnalogWrite(LEDC_CHANNEL_0, ledBright);
  if (ledBright == 255 || ledBright == 0) {
    brightDirection = !brightDirection;
  }
  if (brightDirection) {
    ++ledBright;
  } else {
    --ledBright;
  }
  delay(10);
  Serial.println(ledBright);
}