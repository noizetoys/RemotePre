

//#include <Wire.h>
//#include <SPI.h>
//#include <EEPROM.h>

#include "MicPreController.h"


//#define SWITCH 2

#define ID1 A0
#define ID2 A1
#define ID3 A2
#define ID4 A3
#define ID5 A4

char pinState = LOW;

#define padPin 2
#define phantomPin 5
#define polarityPin 7

MicPreController micPreController;

void setup() {
  Serial.begin(9600);

  pinMode(padPin, INPUT);
  pinMode(phantomPin, INPUT);
  pinMode(polarityPin, INPUT);

  pinMode(ID1, INPUT_PULLUP);
  pinMode(ID2, INPUT_PULLUP);
  pinMode(ID3, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);

  micPreController = MicPreController(setDeviceID());
}


char setDeviceID() {
  Serial.print("Setting Device ID:  ");
  int id1 = !digitalRead(ID1) << 0;
  int id2 = !digitalRead(ID2) << 1;
  int id3 = !digitalRead(ID3) << 2;
  int id4 = !digitalRead(ID4) << 3;
  int id5 = !digitalRead(ID5) << 4;

  int xorbits = id1 ^ id2 ^ id3 ^ id4 ^ id5;
  Serial.println(xorbits);
}


void loop() {
  //  int newValue = digitalRead(padPin);
  //  Serial.print("\nLOOP() --> pin '");
  //  Serial.print(padPin);
  //  Serial.print("' has a value of:  ");
  //  Serial.println(newValue);

  micPreController.updatePad(bool(digitalRead(padPin)));
  micPreController.updatePhantom(bool(digitalRead(phantomPin)));
  micPreController.updatePolarity(bool(digitalRead(polarityPin)));

  delay(100); //small delay to account for button bounce.
}

/*
   Pin State (Pressed/Released)

   Function Button Mode (Pressed/Released)
   Function Button State (On/Off)

   Function State (active/inactive)
*/
