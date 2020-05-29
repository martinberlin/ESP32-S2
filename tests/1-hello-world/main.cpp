#include "Arduino.h"
int c = 0;
void loop() {
    Serial.printf("c:%d\n",c);
    delay(500);
    ++c;
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hello S2");
}