### Select your test

Just edit platformio.ini and uncomment only one of the src_dir folders to select what example to run:

    #src_dir = tests/1-hello-world
    #src_dir = tests/wifi/1-connect
     src_dir = tests/wifi/server

That will select the src/main.cpp of the folder test/wifi/server and compile it. Not ideal but lets you have many different tests under the same codebase.

## ESP32-S2 tests results

Reference: 
  * ![Works](/svg/ok.svg) Marks that works as expected 
  * ![Fails](/svg/no.svg) Something failed

Testing Arduino-framework in ESP32-S2 other folders may include ESP-IDF snippets. Results so far:

wifi/mdns  ![Works](/svg/ok.svg)

wifi/server  ![Not stable](/svg/no.svg) [Test results](/tests/wifi/server/Test-results.txt)

peripherals/pwm  ![Works](/svg/ok.svg) [Test results](/tests/peripherals/pwm/Test-results.txt)

protocols/udp ![Works](/svg/ok.svg) Working stable. [Test results](/tests/protocols/udp/Test-results.txt)

protocols/udp-decompress-brotli ![Works](/svg/ok.svg) [Test results](/tests/protocols/udp-decompress-brotli/Test-results.txt)

protocols/http-client ![Works](/svg/ok.svg) Working stable. [Test results](/tests/protocols/http-client/Test-results.txt)

peripherals/I2S  - Pending test with [Neopixels library](https://github.com/martinberlin/Remora/issues/8)

peripherals/i2c ![Works](/svg/ok.svg) - Seems to work, but I could not connect my Bosch BMP280 sensor, also not with ESP32 maybe is just broken. Just try the **i2cscanner** and try to connect some I2C device on the right GPIOs

peripherals/spi-slave ![Works](/svg/ok.svg) For testing I tried [esp32-spi-slave repository](https://github.com/martinberlin/esp32-spi-slave)

peripherals/spi-master ![Works](/svg/ok.svg) Not tested here but using epaper displays. It works stable and fast just like ESP32

Note about WiFi connection: 

At the point of running this tests I was also compiling and testing other things with the ESP-IDF framework.
On arduino-espressif32 I find the WiFi generally taking more time to connect but not only on S2 and also in ESP32. Thing like this are common:

    ........[W][WiFiGeneric.cpp:810] _eventCallback(): Reason: 202 - AUTH_FAIL
    .....................................................................................ESP-ROM:esp32s2-rc4-20191025
    Build:Oct 25 2019 rst:0x1 (POWERON)

So it does an Auth fail at the beginning even though the credentials are correct. In one case out of 10 it will try to connect and never connect. That's why I usually add a timer and if it does not connect in first 10 seconds I just restart it. Can be my own infrastructure though, just interested to know if someone has experienced the same, if so then just drop me a message.

**Next tests coming:**

More protocols.

**Please note**

My Test results are based on benchmarks I do at home sending with my own apps or connecting with the few devices I have on the office. It's just an indicator and it should not be taken as a statement from my side.
ESP32-S2 is not about being faster than ESP32. My goal here is to analyze, check if I can help discover bugs, only as a side job to compare and make a small resume of the best features of each. 


### ESP-PROG debugging pins

ESP_PROG | ESP32S2

    MTDI --> IO41
    MTDO --> IO40
    MTCK --> IO39
    MTMS --> IO42
    GND --> GND

thanks to @meltdown03

### WiFi.onEvent changed

Many programs used WiFi.onEvent to kick off some function when WiFi connects and gets the ip. I suggest rewriting the constants to use the number behind so it works on both.
Events system has been changed in new version. Its not because S2, it is caused by new events system in esp-idf v4.x.
This is new set of events:
https://github.com/espressif/arduino-esp32/blob/esp32s2/libraries/WiFi/src/WiFiGeneric.h#L36-L77 (src: @chegewara )

### About SPI communication

In ESP32S2 there is no HSPI or VSPI. S2 is using **FSPI** 
Here you can find how to use SPI in arduino (code will works with arduino IDE):
https://github.com/chegewara/arduino-as-component/blob/master/adafruit_ili9341/main/main.cpp


**Board used for this tests:** 
[ESP32-S2 Saola board](https://twitter.com/martinfasani/status/1266352305575727105) vs. 
[ESP32 tinyPICO](https://www.tinypico.com)
