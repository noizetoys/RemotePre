
#include "DataDisplay.h"
#include "MicPreData.h"
#include "MicPreStateProcessor.h"
#include <SPI.h>
/*
   Screen
*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Device ID
#define SHIFT_REGISTER_CLOCK_ENABLE_PIN A0  // Clock Enable - Pin 15
#define SHIFT_REGISTER_DATA_PIN A1          // Serial - Pin 9
#define SHIFT_REGISTER_CLOCK_PIN A2         // Clock Pulse - Pin 2
#define SHIFT_REGISTER_LATCH_PIN A3         // Latch/Load - Pin 1


/*
   Buttons
*/
#define phantomButton 2
#define polarityButton 3
#define inputZButton 4
#define padButton 5
#define hpfButton 6


/*
   Encoder
*/
#define encoderPinA 7
#define encoderPinB 8
#define encoderBtn A4

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_CLK 9
#define OLED_MOSI 10
#define OLED_RESET 11
#define OLED_DC 12
#define OLED_CS 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



MicPreStateProcessor micPreStateProcessor;
DataDisplay dataDisplay;
MicPreData micPreData;


void setup() {
  Serial.begin(115200);

  Serial.println(F("\n\n--> setup called <--"));

  // Encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderBtn, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE);

  configure_DeviceID_Pins();

  // Needs to be done twice to remove stale data
  int deviceID = loadIDFromShiftRegister();
  deviceID = loadIDFromShiftRegister();

  Serial.print("Device ID:  ");
  Serial.println(deviceID);

  // micPreData = MicPreData(deviceID());
  micPreData = MicPreData(deviceID);
  micPreStateProcessor = MicPreStateProcessor(&micPreData);
  dataDisplay = DataDisplay(&display, &micPreData);

  // Buttons
  buttonConfig();
  display.begin(SSD1306_SWITCHCAPVCC);

  dataDisplay.displayDeviceID(micPreData.deviceID);

  delay(2000);

  Serial.println(F("\n--> setup END <--"));
}


void buttonConfig() {
  //  Serial.println(F("--> buttonConfig called"));
  pinMode(phantomButton, INPUT);
  pinMode(polarityButton, INPUT);
  pinMode(inputZButton, INPUT);
  pinMode(padButton, INPUT);
  pinMode(hpfButton, INPUT);
}


// int deviceID() {
//   //  Serial.print(F("--> Setting Device ID:  "));
//   int ID0 = A0;
//   int ID1 = A1;
//   int ID2 = A2;
//   int ID3 = A3;
//   int ID4 = 5;

//   pinMode(ID0, INPUT_PULLUP);
//   pinMode(ID1, INPUT_PULLUP);
//   pinMode(ID2, INPUT_PULLUP);
//   pinMode(ID3, INPUT_PULLUP);
//   pinMode(ID4, INPUT_PULLUP);

//   int id0 = !digitalRead(ID0) << 0;
//   int id1 = !digitalRead(ID1) << 1;
//   int id2 = !digitalRead(ID2) << 2;
//   int id3 = !digitalRead(ID3) << 3;
//   int id4 = !digitalRead(ID4) << 4;

//   return id0 ^ id1 ^ id2 ^ id3 ^ id4;
// }


// Encoder

void doEncoder() {
  micPreStateProcessor.updateGainLevel(digitalRead(encoderPinA), digitalRead(encoderPinB));
}


// Loop

bool firstRun = true;

void loop() {
  if (firstRun) {
    Serial.println(F("\nLoop Started!"));
    firstRun = false;
  }

  micPreStateProcessor.updateMute(bool(digitalRead(encoderBtn)));

  micPreStateProcessor.updatePad(bool(digitalRead(padButton)));
  micPreStateProcessor.updatePhantom(bool(digitalRead(phantomButton)));
  micPreStateProcessor.updatePolarity(bool(digitalRead(polarityButton)));
  micPreStateProcessor.updateInputZ(bool(digitalRead(inputZButton)));
  micPreStateProcessor.updateHPF(bool(digitalRead(hpfButton)));

  dataDisplay.updateDisplay();

  delay(250);
}


// DEVICE ID

void configure_DeviceID_Pins() {
  pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);

  pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);

  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);
  delayMicroseconds(5);
}


int loadIDFromShiftRegister() {
  // Pulse Pins
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
  delayMicroseconds(5);

  digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);

  // Use LSBFirst if displaying the bits for debugging ONLY
  // byte incoming = shiftIn(pin, SHIFT_REGISTER_CLOCK_PIN, LSBFIRST);

  byte incoming = shiftIn(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, MSBFIRST);
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, HIGH);

  return incoming;
}
