
#include "MicPreController.h"

// Buttons
#define phantomButton 8
#define polarityButton 9
#define inputZButton 10
#define padButton 11
#define hpfButton 12

// Encoder
#define encoder0PinA 2  // Gain 
#define encoder0PinB 3
#define encoder0Btn 4   // Pad

#define EncoderMin 1
#define EncoderMax 120
#define EncoderStep 8

volatile int encoder0Pos = 0;
volatile int AValue;
volatile int BValue;
volatile int rotationStep, newRotationStep, btn;


/*
   Screen
*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*
   Main Control
*/
MicPreController micPreController;

/*
   For Debug
*/
bool showEncoderDebug = true;
bool showButtonDebug = true;
bool showDeviceIDDebug = true;
//bool showEncoderDebug = true;



void setup() {
  Serial.begin(115200);
  Serial.println("SetUp Started....");

  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 Allocation Failed!!!");
    for (;;);
  }
 else {
    Serial.println("---> SSD1306 Allocated!!!");
  }

  delay(2000);

  micPreController = MicPreController(setDeviceID(), &display);

  encoderConfig();

  buttonConfig();

  Serial.println("setUp complete!");
}


void buttonConfig() {
  if (showButtonDebug) {
    Serial.println("Configuring Buttons");
  }

  pinMode(phantomButton, INPUT);
  pinMode(polarityButton, INPUT);
  pinMode(inputZButton, INPUT);
  pinMode(padButton, INPUT);
  pinMode(hpfButton, INPUT);
}


void encoderConfig() {
  if (showEncoderDebug) {
    Serial.println("Configuring Buttons");
  }

  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE);
}


char setDeviceID() {
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

  int deviceID = id1 ^ id2 ^ id3 ^ id4 ^ id5;

  if (showDeviceIDDebug) {
    Serial.print("Setting Device ID:  ");
    Serial.println(deviceID);
  }

  return deviceID;
}


void loop() {
  micPreController.updateMute(bool(digitalRead(encoder0Btn)));

  micPreController.updatePhantom(bool(digitalRead(phantomButton)));
  micPreController.updatePolarity(bool(digitalRead(polarityButton)));
  micPreController.updateInputZ(bool(digitalRead(inputZButton)));
  micPreController.updatePad(bool(digitalRead(padButton)));
  micPreController.updateHPF(bool(digitalRead(hpfButton)));


  // Encoder
  //  micPreController.updatePad(bool(!digitalRead(encoder0Btn)));

  delay(200); //small delay to account for button bounce.
}



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
