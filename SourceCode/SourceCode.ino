#include <Adafruit_NeoPixel.h>

#define PIN 17
#define BUTTON_PIN 5
#define NUMPIXELS 7
#define PauseTime 50
#define Number_OF_Programms 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint16_t currentProgram = 1;
bool lastButtonState = LOW;

uint32_t Magenta    = pixels.Color(255, 0, 255);
uint32_t Green      = pixels.Color(50, 205, 50);
uint32_t LimeGreen  = pixels.Color(50, 205, 50);
uint32_t Orange     = pixels.Color(255, 128, 0);

// Names for display in Serial Monitor
const char* programNames[] = {"", "Off","Rainbow","Magenta","Green","LimeGreen","Orange"};

//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);

  pixels.begin();
  pixels.setBrightness(5);
  pixels.clear();
  pixels.show();}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  switch (currentProgram) {
    case 1://OFF
      pixels.clear();
      pixels.show();
      CheckBoopSensor();
      break;
    case 2: rainbowCycle(20);break;
    case 3: SetAllLedsTo(Magenta);break;
    case 4: SetAllLedsTo(Green);break;
    case 5: SetAllLedsTo(LimeGreen);break;
    case 6: SetAllLedsTo(Orange);break;
    default:currentProgram = 1;
  }
}

void CheckBoopSensor() {
  bool currentState = digitalRead(BUTTON_PIN);

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


void SetAllLedsTo(uint32_t color) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
  CheckBoopSensor();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Single Colour Pulsating
// #ToDo

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Rainbow Cycle
void rainbowCycle(int DelayDuration) {
  byte* c;
  uint16_t i, j, currentPixel;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < NUMPIXELS; i++) {
      c = Wheel(((i * 256 / NUMPIXELS) + j) & 255);
      currentPixel = (NUMPIXELS - 1 - i);
      pixels.setPixelColor(currentPixel, pixels.Color(*c, *(c + 1), *(c + 2)));}
    pixels.show();
    CheckBoopSensor();
    if (currentProgram != 2) return; // Exit immediately if program changed
    delay(DelayDuration);  }}

byte* Wheel(byte WheelPosition) {
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
