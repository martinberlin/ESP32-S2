#include <Arduino.h>
#include <SPI.h> // Not used just because is requirement of Adafruit BMP280
#include <Wire.h>
#define I2CFREQ 100000

void setup()
{
  Serial.begin (115200);  
  pinMode(SDA,INPUT_PULLUP);
  pinMode(SCL,INPUT_PULLUP);
  Serial.printf("I2C sda: %d sdl: %d I2CFREQ: %d\n", SDA, SCL, I2CFREQ);

  Wire.begin(SDA, SCL, I2CFREQ);   // sda= GPIO_21 /scl= GPIO_22
}

void Scanner ()
{
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)    // Receive 0 = success (ACK response) 
    {
      Serial.print ("Found address: ");
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);     // PCF8574 7 bit address
      Serial.println (")");
      count++;
    }
  }
  Serial.print ("Found ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
}

void loop()
{
  Scanner();
  delay (500);
}

