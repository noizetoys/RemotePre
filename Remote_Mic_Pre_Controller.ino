

//#include <Wire.h>
//#include <SPI.h>
//#include <EEPROM.h>


// Gain Encoder
// Pad Button
// 48v button
// Polarity Button
// Input Z Button
// Mute Button
// HPF Button

// OLED Display (SPI)
// Raspberry Pi Controller (I2C)

// ID Dipswitches
// Autolevel dipswitch

/*
   Display Items
   Gain Level
   'db' marking
   Pad
   48V
   Polarity
   Input Z
   Mute
   HPF
   Device ID
   Audio Level
*/

#define switchPin 2

#define ID1 5
#define ID2 6
#define ID3 7
#define ID4 8
#define ID5 9

#define UP LOW
#define DOWN HIGH

#define ON HIGH
#define OFF LOW

//int switchState = UP;
//int previousSwitchState = UP;
//int buttonState = UP;
//bool somethingOn = false;

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

  //  switchState = UP;
  //  previousSwitchState = UP;
  //  buttonState = OFF;
  //  somethingOn = false;

  //  Serial.print("LOW == ");
  //  Serial.println(LOW);
  //  Serial.print("HIGH == ");
  //  Serial.println(HIGH);
  //
  //  Serial.print("UP == ");
  //  Serial.println(UP);
  //  Serial.print("DOWN == ");
  //  Serial.println(DOWN);
  //
  //  Serial.print("OFF == ");
  //  Serial.println(OFF);
  //  Serial.print("ON == ");
  //  Serial.println(ON);
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
    Serial.println("\nButton has been released");

    thingIsOn = thingIsOn == true ? false : true;
    Serial.print("Thing Is On?    ");
    Serial.println(thingIsOn == true ? "YES" : "NO");
  }
  
  delay(100); //small delay to account for button bounce.
}
