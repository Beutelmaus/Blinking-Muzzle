<p align="center">
<img src="/doc/images/Projekt_Logo.jpg" width="600">
</p>

Blinking-Muzzle is a silly side project to play around with some LED animation.

# Hardware

I based this project around the [Adafruit SCORPIO](https://github.com/adafruit/Adafruit-Feather-RP2040-SCORPIO-PCB) because of it's 8 independent output strings and a included lipo charger.

The exact hardware selection is irrelevant but my choice is

- (Feather RP2040 SCORPIO - 8 Channel NeoPixel Driver)[https://www.adafruit.com/product/5650]
- (NeoPixel Mini Button)https://www.adafruit.com/product/4356]
- (Lithium Ion Battery - 3.7V 2000mAh)[https://www.adafruit.com/product/2011]
- (Resin printed spikes)[https://cad.onshape.com/documents/40bc9dfccdbb5360ae246c25/w/3191a320dfa877ba2eaf8838/e/0eacd3ec2333142fc41c1fc5]
- (Capacitive Touch Sensor - AT42QT1070)[https://www.adafruit.com/product/1362]
- A switch, I use a DPDT
- Some wires and solder to connect everything


# Install

The SCORPIO RP2040 is capable of running CircuitPython or being configured by the arduino ide, my choice fell on the latter.
1. The IDE does not support the board out of the box.
    The (Hardware Libary)[https://github.com/earlephilhower/arduino-pico] for the (Board)[https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json] must be  installed
    For ArduinoIDE V1-* list it in File-->Preferences-->Aditional boards and selected in Tools-->Board-->Boardmanager.
    For ArduinoIDE V2.* follow (Official guide)[https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/]
2. The (Neopixel)[https://github.com/adafruit/Adafruit_NeoPixel] or for multiple strings the (NeoPXL8)[https://github.com/adafruit/Adafruit_NeoPXL8] libary must be (installed)[https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/]
3. The firmware has to be flashed for program download.
    To do this, hold the boot button and press the reset key.
    After the Scorpio mounts as an external drive, the program can be transferred.
    This step is not required for any further downloads.
    
# Code

The *void loop()* uses a switch statement to select a funcion to call.
Each mode function updates the LED strip with a specific color or animation, and then calls CheckBoopSensor().

The CheckBoopSensor() function monitors the hardware input.
It increments the current program number when the capacitive sensor is detects a **Boop**.
A debug message is also sent to the Serial Monitor showing the button state and the selected program.

# Wiring

The layout is very similat to the desing of adafruit(Adafruit)[https://learn.adafruit.com/introducing-feather-rp2040-scorpio/lipoly-battery-jst]
<p align="center">
<img src="/doc/images/Wiring-Breadboard.png" width="600">
</p>
<p align="center">
<img src="/doc/images/Wiring-Schematic.png" width="600">
</p>

# Project dependencies

This project relies on several third party Software:

- Arduino IDE
- (Hardware Libary)[https://github.com/earlephilhower/arduino-pico]
- (Neopixel)[https://github.com/adafruit/Adafruit_NeoPixel] or (NeoPXL8)[https://github.com/adafruit/Adafruit_NeoPXL8] libary must be (installed)[https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/] libary

Honorable mentions:
- some silly guy made this thing first, adding led's to everything is not my idea
- The RainbowCycle code is a copie from (Electriangle)[https://github.com/Electriangle/RainbowCycle_Main]

# License

- MIT
