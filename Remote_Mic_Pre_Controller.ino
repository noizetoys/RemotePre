
#include "MicPreController.h"

// Buttons
#define padPin 2
#define phantomPin 3
#define polarityPin 4
#define inputZPin 5
//#define mutePin 6
#define highPassFilterPin 6

// Encoder
#define encoder0PinA 2  // Gain 
#define encoder0PinB 3
#define encoder0Btn 4   // Pad

volatile int encoder0Pos = 0;


MicPreController micPreController;


void setup() {
  Serial.begin(115200);

  // Buttons
  //  pinMode(padPin, INPUT);
  //  pinMode(phantomPin, INPUT);
  //  pinMode(polarityPin, INPUT);
  //  pinMode(inputZPin, INPUT);
  //  pinMode(highPassFilterPin, INPUT);

  // Encoder
  encoderConfig();
  micPreController = MicPreController(setDeviceID());
  //  micPreController = MicPreController(17);
  Serial.println("setUp complete!");
}


void buttonConfig() {
  
}


void encoderConfig() {
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE);
}


char setDeviceID() {
  Serial.print("Setting Device ID:  ");
  const int ID1 = A0;
  const int ID2 = A1;
  const int ID3 = A2;
  const int ID4 = A3;
  const int ID5 = 5;

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


volatile int rotationStep, newRotationStep, btn;


void loop() {
  //  micPreController.updatePad(bool(digitalRead(padPin)));

  //  micPreController.updatePhantom(bool(digitalRead(phantomPin)));
  //  micPreController.updatePolarity(bool(digitalRead(polarityPin)));
  //  micPreController.updateInputZ(bool(digitalRead(inputZPin)));
  micPreController.updateMute(bool(digitalRead(mutePin)));
  //  micPreController.updateHPF(bool(digitalRead(highPassFilterPin)));


  // Encoder
  //  micPreController.updatePad(bool(!digitalRead(encoder0Btn)));

  delay(200); //small delay to account for button bounce.
}


volatile int AValue;
volatile int BValue;

#define EncoderMin 1
#define EncoderMax 60
#define EncoderStep 5


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
    //    Serial.println("\n \t ~~~~~> NEW rotationStep SET !!!!!!!!!!");
    //    Serial.print("\tOld Rotation Step = ");
    //    Serial.print(rotationStep);
    //    Serial.print(", NEW Rotation Step = ");
    //    Serial.println(newRotationStep);
    rotationStep = newRotationStep;

    Serial.print("\nRotationStep = ");
    Serial.println(rotationStep);

  }


  if (encoder0Pos < EncoderMin) {
    //    Serial.print(EncoderMin);
    //    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
    //    Serial.print(EncoderMax);
    //    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

}
