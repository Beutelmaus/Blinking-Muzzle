/*The Parent repo: https://github.com/Beutelmaus/Blinking-Muzzle
 *
* RESOURCES:
* NeoPixel Uberguide: https://learn.adafruit.com/adafruit-neopixel-uberguide
* NeoPXL8 Strandtest:https://github.com/adafruit/Adafruit_NeoPXL8/blob/master/examples/NeoPXL8/strandtest/strandtest.ino
*/


#include <Adafruit_NeoPXL8.h>

#define NUM_LEDS    7      // NeoPixels PER STRAND, total number is 8X this!

// For the Feather RP2040 SCORPIO, use this list:
int8_t pins[8] = { 16, 17, 18, 19, 20, 21, 22, 23 };

#define BUTTON_PIN 5
#define Number_OF_Programms 6

Adafruit_NeoPXL8 leds(NUM_LEDS, pins, NEO_GRB + NEO_KHZ800);



uint16_t currentProgram = 1;
const char* programNames[] = {"", "Off","Rainbow","Magenta","Green","Cyan","Red"};// Names for display in Serial Monitor
uint16_t ProgramLastCycle = 1;
bool lastButtonState = LOW;

byte Brightness_Low       =  3;
byte Brightness_High      = 20;
byte Brightness_Selected  = 20;

static uint8_t colors[8][3] = {
  255,   0,   0, // Row 0: Red
  255, 160,   0, // Row 1: Orange
  255, 255,   0, // Row 2: Yellow
    0, 255,   0, // Row 3: Green
    0, 255, 255, // Row 4: Cyan
    0,   0, 255, // Row 5: Blue
  192,   0, 255, // Row 6: Purple
  255,   0, 255  // Row 7: Magenta
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);

  if (!leds.begin()) {// Blink the onboard LED if something was configurn wrong.
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  leds.setBrightness(10);
  
  for (uint32_t color = 0xFF0000; color > 0; color >>= 8) {// Cycle all pixels red/green/blue on startup. 
    leds.fill(color);
    leds.show();
    delay(500);
  }
  currentProgram = 1;//Start with all LED's off
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  switch (currentProgram) {
    case 1://OFF
      leds.clear();
      leds.show();
      CheckBoopSensor();
      break;
    case 2: RainbowCycle(20);break;
    case 3: SetAllLedsTo(colors[7]);break;//Magenta
//    case 4: PulseColour(20, Green);break;
    case 4: SetAllLedsTo(colors[3]);break;//Green
    case 5: SetAllLedsTo(colors[4]);break;//Cyan
    case 6: SetAllLedsTo(colors[0]);break;//Red
    default:currentProgram = 1;
  }
}

void CheckBoopSensor() {
  bool currentState = digitalRead(BUTTON_PIN);
  ProgramLastCycle  = currentProgram;
  // Print both sensor state and current program
  Serial.print("Button: ");
  Serial.print(currentState == HIGH ? "Off" : "On");
  Serial.print(" | Program: ");
  Serial.print(currentProgram);
  Serial.print(" - ");
  Serial.println(programNames[currentProgram]);

  if (currentState == LOW && lastButtonState == HIGH) {
    currentProgram++;
    if (currentProgram > Number_OF_Programms) currentProgram = 1;
    }

  lastButtonState = currentState;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Single Colour

void SetAllLedsTo(uint8_t color[3]) {
  uint32_t _color = leds.Color(color[0], color[1], color[2]);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i           , _color);// Strip 1
    leds.setPixelColor(i + NUM_LEDS, _color);// Strip 2
    // Add more strips if needed...
  }

  leds.show();
  CheckBoopSensor();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Rainbow Cycle
void RainbowCycle(int DelayDuration) {
  byte* c;
  uint16_t i, j, currentPixel;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < NUM_LEDS; i++) {
      c = RainbowWheel(((i * 256 / NUM_LEDS) + j) & 255);
      currentPixel = (NUM_LEDS - 1 - i);
      leds.setPixelColor(currentPixel           , leds.Color(*c, *(c + 1), *(c + 2)));//Strip 1
      leds.setPixelColor(currentPixel + NUM_LEDS, leds.Color(*c, *(c + 1), *(c + 2)));//Strip 2
   }
    leds.show();
    CheckBoopSensor();
    if (currentProgram != ProgramLastCycle) return; // Exit immediately if program changed
    delay(DelayDuration);  }}

byte* RainbowWheel(byte WheelPosition) {
  static byte c[3];
  if (WheelPosition < 85) {
    c[0] = WheelPosition * 3;
    c[1] = 255 - WheelPosition * 3;
    c[2] = 0;
  } else if (WheelPosition < 170) {
    WheelPosition -= 85;
    c[0] = 255 - WheelPosition * 3;
    c[1] = 0;
    c[2] = WheelPosition * 3;
  } else {
    WheelPosition -= 170;
    c[0] = 0;
    c[1] = WheelPosition * 3;
    c[2] = 255 - WheelPosition * 3;  }
  return c;}
