
#include "DataDisplay.h"
#include "MicPreData.h"
#include "MicPreStateProcessor.h"

/*
   Screen
*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*
   Encoder
*/
#define encoderPinA 2
#define encoderPinB 3
#define encoderBtn 4

/*
   Buttons
*/
#define phantomButton 8
#define polarityButton 9
#define inputZButton 10
#define padButton 11
#define hpfButton 12


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

  micPreData = MicPreData(deviceID());
  micPreStateProcessor = MicPreStateProcessor(&micPreData);
  dataDisplay = DataDisplay(&display, &micPreData);

  // Buttons
  buttonConfig();

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


int deviceID() {
  //  Serial.print(F("--> Setting Device ID:  "));
  int ID0 = A0;
  int ID1 = A1;
  int ID2 = A2;
  int ID3 = A3;
  int ID4 = 5;

  pinMode(ID0, INPUT_PULLUP);
  pinMode(ID1, INPUT_PULLUP);
  pinMode(ID2, INPUT_PULLUP);
  pinMode(ID3, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);

  int id0 = !digitalRead(ID0) << 0;
  int id1 = !digitalRead(ID1) << 1;
  int id2 = !digitalRead(ID2) << 2;
  int id3 = !digitalRead(ID3) << 3;
  int id4 = !digitalRead(ID4) << 4;

  return id0 ^ id1 ^ id2 ^ id3 ^ id4;
}


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
