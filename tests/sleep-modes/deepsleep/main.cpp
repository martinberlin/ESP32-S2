#include "Arduino.h"
#ifdef TINYPICO
    #include <TinyPICO.h>
#endif

unsigned int secondsToDeepsleep = 20;
uint64_t USEC = 1000000;

void setup() {
  Serial.begin(115200);
  Serial.println("setup() go to deepsleep in 1 second");
  // the DIN pin of the Neopixel RGB LED: https://dl.espressif.com/dl/schematics/ESP32-S2-SAOLA-1_V1.1_schematics.pdf (page 2)
  pinMode(18, OUTPUT);

  delay(1000);
  #ifdef TINYPICO
    TinyPICO tp = TinyPICO();
    tp.DotStar_SetPower(false);
    float batteryVoltage = tp.GetBatteryVoltage();
    Serial.printf("TINYPICO version. Battery: %.6f v\n", batteryVoltage);
    #elif S2
    // Turn low all gpios just in case even though this should be already low in deepsleep (Didn't changed the 0.79 mA consumption at all)
    for (int8_t gpio = 0; gpio <= 46; ++gpio) {
       digitalWrite(gpio, LOW);
    }
    delay(2);
  #endif
    esp_sleep_enable_timer_wakeup(secondsToDeepsleep * USEC);
    Serial.println("Deepsleeping now");
    esp_deep_sleep_start();
}
 
void loop() {
}