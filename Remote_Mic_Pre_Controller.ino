

//#include <Wire.h>
//#include <SPI.h>
//#include <EEPROM.h>

#include "MicPreController.h"


#define switchPin 2

#define ID1 5
#define ID2 6
#define ID3 7
#define ID4 8
#define ID5 9

int pinState = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  //  pinMode(switchPin, INPUT_PULLUP);

  pinMode(ID1, INPUT_PULLUP);
  pinMode(ID2, INPUT_PULLUP);
  pinMode(ID3, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);
  pinMode(ID4, INPUT_PULLUP);

  setDeviceID();
}


void setDeviceID() {
  Serial.println("Setting Device ID");
  int id1 = !digitalRead(ID1) << 0;
  int id2 = !digitalRead(ID2) << 1;
  int id3 = !digitalRead(ID3) << 2;
  int id4 = !digitalRead(ID4) << 3;
  int id5 = !digitalRead(ID5) << 4;

  int xorbits = id1 ^ id2 ^ id3 ^ id4 ^ id5;
  Serial.println("XOR'ed Bit shift");
  Serial.println(xorbits);
}



#define PRESSED 1
#define RELEASED 0

int button_mode = RELEASED;
bool thingIsOn = false;

void loop() {
  pinState = digitalRead(switchPin);
  
  if ((pinState == PRESSED) && (button_mode == RELEASED)) {
    button_mode = PRESSED;
    
    Serial.println("\nButton has been pressed.");
  }
  else if ((pinState == RELEASED) && (button_mode == PRESSED)) {
    button_mode = RELEASED;
    Serial.println("Button has been released");

    thingIsOn = thingIsOn == true ? false : true;
    Serial.print("Thing Is On? ");
    Serial.println(thingIsOn == true ? "YES" : "NO");
  }
  
  delay(100); //small delay to account for button bounce.
}
