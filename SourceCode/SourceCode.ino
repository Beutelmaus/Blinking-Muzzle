/*The Parent repo: https://github.com/Beutelmaus/Blinking-Muzzle
 *
* RESOURCES:
* NeoPixel Uberguide: https://learn.adafruit.com/adafruit-neopixel-uberguide
* ScorpioRP2040_Pinout: https://learn.adafruit.com/introducing-feather-rp2040-scorpio/pinouts
* NeoPXL8 Strandtest:https://github.com/adafruit/Adafruit_NeoPXL8/blob/master/examples/NeoPXL8/strandtest/strandtest.ino
*/

#include <Adafruit_NeoPXL8.h>//Strips
#include <Adafruit_NeoPixel.h>//Onboard_LED
Adafruit_NeoPixel OnboardLED(1, 4, NEO_GRB + NEO_KHZ800);

#define NUM_LEDS    9      // NeoPixels PER STRAND, total number is 8X this!
int8_t pins[8] = {16, 17, 18, 19, 20, 21, 22, 23 };
Adafruit_NeoPXL8 leds(NUM_LEDS, pins, NEO_GRB + NEO_KHZ800);


#define BoopSensorIn 6
#define BoopSensorOut 5

#define BrightnesSensorIn 10
#define BrightnesSensorOut 12

#define Number_OF_Programms 6




uint16_t currentProgram = 1;
const char* programNames[] = {"", "Off","Rainbow","Magenta","Green","Cyan","Red"};// Names for display in Serial Monitor
uint16_t ProgramLastCycle = 1;
bool lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
bool Brightness_lastState = LOW;
uint16_t Brighness_Level = 1;
byte Brightness_Selected  = 250;
byte Brightness_LastCycle  = 250;

uint16_t i, j;//Used in For loops

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
  pinMode(BoopSensorIn, INPUT);
  pinMode(BoopSensorOut, OUTPUT);
  digitalWrite(BoopSensorOut, 0);
  pinMode(BrightnesSensorIn, INPUT);
  pinMode(BrightnesSensorOut, OUTPUT);
  digitalWrite(BrightnesSensorOut, 1);
  
  Serial.begin(9600);

  if (!leds.begin()) {// Blink the onboard LED if something was configurn wrong.
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;){
      digitalWrite(LED_BUILTIN, (millis() / 1000) & 1);
      Serial.println("Configuration Error");
    }
  }
  leds.setBrightness(10);
  
  for (uint32_t color = 0xFF0000; color > 0; color >>= 8) {// Cycle all pixels red/green/blue on startup. 
    leds.fill(color);
    leds.show();
    delay(1500);
    digitalWrite(BoopSensorOut, 1);//Sensor teaches value when switching on
  }
  currentProgram = 1;//Start with all LED's off

  OnboardLED.begin();
  OnboardLED.setBrightness(10);
  OnboardLED.show();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  switch (currentProgram) {
    case 1://OFF
      leds.clear();
      leds.show();
      CheckBoopSensor();

      OnboardLED.clear();
      OnboardLED.show();
      break;
    case 2: rainbow(50);break;//Pass cycle time in ms
    case 3: WavePulseAllLeds(colors[7], 40,  750);break;//Magenta, 40%, 0,75s
    case 4: WavePulseAllLeds(colors[3], 10, 3000);break;//Green
    case 5: WavePulseAllLeds(colors[4], 80, 5000);break;//Cyan
    case 6: PulseAllLeds(colors[0], 50, 10000);break;//Red
    default:currentProgram = 1;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckBoopSensor() {
  bool currentState = digitalRead(BoopSensorIn);
  ProgramLastCycle  = currentProgram;
 
  //Debug Output
  Serial.print("Button: ");      Serial.print(currentState == HIGH ? "Off" : "On");
  CheckBrightnessSensor();  Serial.print(" | Brightness: ");  Serial.print(Brighness_Level);
  Serial.print(" | Program: ");  Serial.print(currentProgram);
  Serial.print(" - ");           Serial.println(programNames[currentProgram]);

  if (currentState == LOW && lastButtonState == HIGH) {
    currentProgram++;
    if (currentProgram > Number_OF_Programms) currentProgram = 1;
    }

  lastButtonState = currentState;
  CheckBrightnessSensor();
  SetOnboardLed();
}

void CheckBrightnessSensor() {
  bool Brightness_currentState = digitalRead(BrightnesSensorIn);

    if (Brightness_currentState == HIGH && Brightness_lastState == LOW) {
      Brighness_Level++;
      if (Brighness_Level > 4) Brighness_Level = 1;
  }

  Brightness_lastState = Brightness_currentState;
}
void SetOnboardLed() {
  uint32_t color = leds.getPixelColor(0); // First pixel of first strip
  OnboardLED.setPixelColor(0, color);
  OnboardLED.show();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Single Colour

void SetAllLedsTo(uint8_t color[3]) {
  uint32_t _color = leds.Color(color[0], color[1], color[2]);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i           , _color);// Strip 1
    leds.setPixelColor(i + NUM_LEDS, _color);// Strip 2
    // fill command would do the same but this is function is usefull for testing
  }
  leds.show();
  CheckBoopSensor();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Single Pulsating

void PulseAllLeds(uint8_t color[3], uint8_t startPercent, uint16_t cycleTime) {
  if (startPercent > 100){currentProgram = currentProgram + 1; return;}//Skip program if call is invalid
  uint16_t delayPerStep = cycleTime / (2 * (100 - startPercent)); // Divide by 2 for fade in + out

  // Fade in
  for (uint8_t i = startPercent; i <= 100; i += 1) {
    uint8_t r = color[0] * i / 100;
    uint8_t g = color[1] * i / 100;
    uint8_t b = color[2] * i / 100;

    leds.fill(leds.Color(r, g, b));
    leds.show();
    CheckBoopSensor();
    if (currentProgram != ProgramLastCycle) return; // Exit immediately if program changed
    delay(delayPerStep);
  }

  // Fade out
  for (int i = 100; i >= startPercent; i -= 1) {
    uint8_t r = color[0] * i / 100;
    uint8_t g = color[1] * i / 100;
    uint8_t b = color[2] * i / 100;

    leds.fill(leds.Color(r, g, b));
    leds.show();
    CheckBoopSensor();
    if (currentProgram != ProgramLastCycle) return; // Exit immediately if program changed
    delay(delayPerStep);
  }
  Serial.println(" Finished Single Pulse Cycle");}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Pulsating Wave
void WavePulseAllLeds(uint8_t color[3], uint8_t startPercent, uint16_t cycleTime) {
  if (startPercent > 100){currentProgram = currentProgram + 1; return;}//Skip program if call is invalid

 const uint8_t maxBrightness = 100;
  const uint16_t StepsInTotal = 255; // Number of wave steps
  const uint16_t delayPerStep = cycleTime / StepsInTotal;
  const float twoPi = 3.14159265359 *2;

  for (uint16_t step = 0; step < StepsInTotal; step++) {
    for (uint8_t i = 0; i < NUM_LEDS *2 ; i++) {
      
      float phase = ((float)i / (NUM_LEDS * 2)) * twoPi;//Calculate LED-specific phase offset
      float brightness = 0.5 * (1 + sin(twoPi * step / StepsInTotal + phase));//Use sin to make a wave, normalized to 0–100%

      float scaledBrightness = brightness * (maxBrightness - startPercent) + startPercent;

      uint8_t r = color[0] * scaledBrightness / 100;
      uint8_t g = color[1] * scaledBrightness / 100;
      uint8_t b = color[2] * scaledBrightness / 100;

    leds.setPixelColor(i           , leds.Color(r, g, b));// Strip 1
    leds.setPixelColor(i + NUM_LEDS, leds.Color(r, g, b));// Strip 2
    }

    leds.show();
    CheckBoopSensor();
    if (currentProgram != ProgramLastCycle) return;
    delay(delayPerStep);
  }
  Serial.println(" Finished Pulsating Wave Cycle");}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Rainbow Cycle
 void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<NUM_LEDS; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / NUM_LEDS);
      leds.setPixelColor(i           , leds.gamma32(leds.ColorHSV(pixelHue)));//Strip 1
      leds.setPixelColor(i + NUM_LEDS, leds.gamma32(leds.ColorHSV(pixelHue)));//Strip 2
    }
    leds.show();
    CheckBoopSensor();
    if (currentProgram != 2) return;
    delay(wait); 
  }
}
