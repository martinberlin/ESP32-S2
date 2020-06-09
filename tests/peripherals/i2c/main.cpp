#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bmp; // I2C

#define P0 1013.25


void setup()
{
  Serial.begin(115200);  
  pinMode(SDA,INPUT_PULLUP);
  //pinMode(SCL,INPUT_PULLUP);
  Serial.printf("I2C sda: %d sdl: %d\n", SDA, SCL);

    if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BME280::MODE_NORMAL);
}

void loop() {
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
    delay(2000);
}