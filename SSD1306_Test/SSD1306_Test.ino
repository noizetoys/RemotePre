//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include "GainEncoder.h"
#include "DataReadout.h"
#include "DisplayData.h"

/*
   Screen
*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*
   Encoder
*/
#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4

#define EncoderMin 1
#define EncoderMax 120
#define EncoderStep 8

volatile int encoder0Pos = 0;
volatile int AValue;
volatile int BValue;
volatile int rotationStep, newRotationStep, btn;


/*
   Buttons
*/
#define phantomButton 8
#define polarityButton 9
#define inputZButton 10
#define padButton 11
#define hpfButton 12

/*
   ID DIP Switches

  #define ID0 A0
  #define ID1 A1
  #define ID2 A2
  #define ID3 A3
  #define ID4 A4
*/


GainEncoder encoder;
DataReadout dataReadout;
DisplayData displayData;

void setup() {
  Serial.begin(115200);

  //  encoder = GainEncoder();
  //    attachInterrupt(0, encoder.readEncoder, CHANGE);

  //  attachInterrupt(0, encoder.readEncoder, CHANGE);

 
  // Buttons
  buttonConfig();

  // Encoder
  encoderConfig();

  // Device ID
  deviceIDConfig();


  displayData = DisplayData();

  dataReadout = DataReadout(&display, &displayData);
}


void encoderConfig() {
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);

  attachInterrupt(0, doEncoder, CHANGE);
}


void buttonConfig() {
  pinMode(phantomButton, INPUT);
  pinMode(polarityButton, INPUT);
  pinMode(inputZButton, INPUT);
  pinMode(padButton, INPUT);
  pinMode(hpfButton, INPUT);
}


void deviceIDConfig() {
  Serial.print("Setting Device ID:  ");
  const int ID0 = A0;
  const int ID1 = A1;
  const int ID2 = A2;
  const int ID3 = A3;
  const int ID4 = 5;

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

  int xorbits = id0 ^ id1 ^ id2 ^ id3 ^ id4;
  Serial.println(xorbits);
}


/*
   Encoder
*/

void doEncoder() {
  AValue = digitalRead(encoder0PinA);
  BValue = digitalRead(encoder0PinB);

  if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
    AValue == BValue ? encoder0Pos++ : encoder0Pos--;
  }

  //  Serial.print("doEncoder:  Current Encoder Value = ");
  //  Serial.println(encoder0Pos);

  newRotationStep = (encoder0Pos / EncoderStep);

  if (newRotationStep != rotationStep && encoder0Pos >= EncoderStep) {
    rotationStep = newRotationStep;
    Serial.print("\nRotationStep = ");
    Serial.println(rotationStep);
  }

  if (encoder0Pos < EncoderMin) {
    Serial.print(EncoderMin);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
    Serial.print(EncoderMax);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

  displayData.gainLevel = rotationStep;

//  dataReadout.updateDisplay();
}


/*
   Loop
*/

void loop() {
  displayData.muteButtonState = bool(digitalRead(encoder0Btn));

  displayData.phantomButtonState = bool(digitalRead(phantomButton));
  displayData.polarityButtonState = bool(digitalRead(polarityButton));
  displayData.inputZButtonState = bool(digitalRead(inputZButton));
  displayData.padButtonState = bool(digitalRead(padButton));
  displayData.highPassFilterButtonState = bool(digitalRead(hpfButton));


//  Serial.print("\nEncoder Button State:  ");
//  Serial.println(displayData.muteButtonState == HIGH ? "HIGH" : "LOW");
//
//  Serial.print("Phantom State:  ");
//  Serial.println(displayData.phantomButtonState == HIGH ? "HIGH" : "LOW");
//
//  Serial.print("Polarity State:  ");
//  Serial.println(displayData.polarityButtonState == HIGH ? "HIGH" : "LOW");
//
//  Serial.print("Input Z State:  ");
//  Serial.println(displayData.inputZButtonState == HIGH ? "HIGH" : "LOW");
//
//  Serial.print("Pad State:  ");
//  Serial.println(displayData.padButtonState == HIGH ? "HIGH" : "LOW");
//
//  Serial.print("HPF State:  ");
//  Serial.println(displayData.highPassFilterButtonState == HIGH ? "HIGH" : "LOW");

  if (displayData.muteButtonState == LOW) {
    //  if (digitalRead(MUTE_BUTTON) == HIGH || digitalRead(encoder0Btn) == HIGH) {
//    displayMute();
    Serial.println("MUTE should be Shown!");
  }

  dataReadout.updateDisplay();

  delay(250);
}