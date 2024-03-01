

//#include <Wire.h>
//#include <SPI.h>
//#include <EEPROM.h>

#include "MicPreController.h"


char pinState = LOW;

#define padPin 2
#define phantomPin 3
#define polarityPin 4
#define inputZPin 5
#define mutePin 6
#define highPassFilterPin 7

MicPreController micPreController;

void setup() {
  Serial.begin(9600);

  pinMode(padPin, INPUT);
  pinMode(phantomPin, INPUT);
  pinMode(polarityPin, INPUT);
  pinMode(inputZPin, INPUT);
  pinMode(mutePin, INPUT);
  pinMode(highPassFilterPin, INPUT);

  micPreController = MicPreController(setDeviceID());
}


char setDeviceID() {
  Serial.print("Setting Device ID:  ");
  const int ID1 = A0;
  const int ID2 = A1;
  const int ID3 = A2;
  const int ID4 = A3;
  const int ID5 = A4;

  pinMode(ID1, INPUT_PULLUP);
  pinMode(ID2, INPUT_PULLUP);
  pinMode(ID3, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);

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

//  micPreController.updatePad(bool(digitalRead(padPin)));
//  micPreController.updatePhantom(bool(digitalRead(phantomPin)));
//  micPreController.updatePolarity(bool(digitalRead(polarityPin)));
  micPreController.updateInputZ(bool(digitalRead(inputZPin)));
  micPreController.updateMute(bool(digitalRead(mutePin)));
  micPreController.updateHPF(bool(digitalRead(highPassFilterPin)));

  delay(100); //small delay to account for button bounce.
}

/*
   Pin State (Pressed/Released)

   Function Button Mode (Pressed/Released)
   Function Button State (On/Off)

   Function State (active/inactive)
*/
