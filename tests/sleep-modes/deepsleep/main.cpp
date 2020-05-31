#include "Arduino.h"
#ifdef TINYPICO
    #include <TinyPICO.h>
#endif

unsigned int secondsToDeepsleep = 20;
uint64_t USEC = 1000000;

void setup() {
  Serial.begin(115200);
  Serial.println("setup() go to deepsleep in 1 second");
  delay(1000);
  #ifdef TINYPICO
    TinyPICO tp = TinyPICO();
    tp.DotStar_SetPower(false);
    batteryVoltage = tp.GetBatteryVoltage();
    Serial.printf("TINYPICO version. Battery: %.6f v\n", batteryVoltage);
  #endif
    esp_sleep_enable_timer_wakeup(secondsToDeepsleep * USEC);
    esp_deep_sleep_start();
}
 
void loop() {
}