## ESP32-S2 tests 

Testing Arduino-framework in ESP32-S2 other folders may include ESP-IDF snippets. Results so far:

wifi/mdns  ![Works](/svg/ok.svg)

wifi/server  ![Not stable](/svg/no.svg) [Test results](/tests/wifi/server/Test-results.txt)

peripherals/pwm  ![Works](/svg/ok.svg) [Test results](/tests/peripherals/pwm/Test-results.txt)

protocols/udp ![Works](/svg/ok.svg) Working stable. [Test results](/tests/protocols/udp/Test-results.txt)

peripherals/I2S Not sure, could not test yet, many things changed in 4.0 [Neopixels library does not compile](https://github.com/martinberlin/Remora/issues/8). A lot of libraries will need refactoring to make things work on S2. I will try later the [ESP-IDF example on I2S](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/i2s.html).


REF:
![Works](/svg/ok.svg) Marks that works as expected | ![Fails](/svg/no.svg) Something failed

### Select your test

Just edit platformio.ini and uncomment only one of the src_dir folders to select what example to run:

    #src_dir = tests/1-hello-world
    #src_dir = tests/wifi/1-connect
     src_dir = tests/wifi/server

That will select the src/main.cpp of the folder test/wifi/server and compile it. Not ideal but lets you have many different tests under the same codebase.

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


Board used for this tests:
[ESP32-S2 Saola board](https://twitter.com/martinfasani/status/1266352305575727105)
[ESP32 tinyPICO](https://www.tinypico.com)
