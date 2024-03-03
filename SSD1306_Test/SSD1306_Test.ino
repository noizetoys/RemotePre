#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

int muteButtonState;

/*
   Buttons
*/
#define phantomButton 8
#define polarityButton 9
#define inputZButton 10
#define padButton 11
#define hpfButton 12

int phantomState;
int polarityState;
int inputZState;
int padState;
int hpfState;

/*
   ID DIP Switches

  #define ID0 A0
  #define ID1 A1
  #define ID2 A2
  #define ID3 A3
  #define ID4 A4
*/



void setup() {
  Serial.begin(115200);

  // Display
  displayConfig();

  // Buttons
  buttonConfig();

  // Encoder
  encoderConfig();

  // Device ID
  deviceIDConfig();

  // Starting State
  displayMute();
}


void displayConfig() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 Allocation Failed!!!");
    for (;;);
  }

  delay(2000);
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
   Text Related
*/
void resetText() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);
}


void displayPhantom() {
  resetText();
  display.setCursor(1, 1);

  if (phantomState == HIGH) {
    display.println("48v");
  }
}


void displayPolarity() {
  resetText();

  if (polarityState == HIGH) {
    display.setCursor(41, 1);
    display.println("180");
  }
}


void displayInputZ() {
  resetText();

  display.setCursor(92, 1);
  display.println(inputZState == LOW ? "LOW" : "HI");
}


void displayPad() {
  resetText();

  if (padState == HIGH) {
    display.setCursor(92, 26);
    display.println("PAD");
  }
}


void displayHPF() {
  resetText();

  if (hpfState == HIGH) {
    display.setCursor(92, 50);
    display.println("HPF");
  }
}


void displayGain(int gain) {
  //  resetText();
  display.setTextSize(3);
  display.setCursor(1, 35);
  display.println("+");

  display.setTextSize(5);
  display.setCursor(20, 28);
  display.println(gain);
}


void displayAllItems(int gain) {
  display.clearDisplay();
  display.invertDisplay(false);

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);

  displayPhantom();

  displayPolarity();

  displayInputZ();

  displayPad();

  displayHPF();

  displayGain(gain);

  display.display();
}


void displayMute() {
  display.clearDisplay();
  display.invertDisplay(true);

  display.drawRect(0, 0, 128, 16, WHITE);

  display.setTextColor(WHITE);
  display.setTextSize(5);
  display.setFont(NULL);

  display.setCursor(7, 23);
  display.println("MUTE");
  display.display();

  delay(2000);
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

}


/*
   Loop
*/

void loop() {
  muteButtonState = digitalRead(encoder0Btn);

  phantomState = digitalRead(phantomButton);
  polarityState = digitalRead(polarityButton);
  inputZState = digitalRead(inputZButton);
  padState = digitalRead(padButton);
  hpfState = digitalRead(hpfButton);

  //  Serial.print("Encoder Button State:  ");
  //  Serial.println(muteButtonState == HIGH ? "HIGH" : "LOW");

  //    Serial.print("Phantom State:  ");
  //    Serial.println(phantomState == HIGH ? "HIGH" : "LOW");
  //
  //  Serial.print("Polarity State:  ");
  //  Serial.println(polarityState == HIGH ? "HIGH" : "LOW");
  //
  //  Serial.print("Input Z State:  ");
  //  Serial.println(inputZState == HIGH ? "HIGH" : "LOW");
  //
  //  Serial.print("Pad State:  ");
  //  Serial.println(padState == HIGH ? "HIGH" : "LOW");
  //
  //  Serial.print("HPF State:  ");
  //  Serial.println(hpfState == HIGH ? "HIGH" : "LOW");

  if (digitalRead(encoder0Btn) == LOW) {
    //  if (digitalRead(MUTE_BUTTON) == HIGH || digitalRead(encoder0Btn) == HIGH) {
    displayMute();
    Serial.println("MUTE should be Shown!");
  }
  else {
    displayAllItems((rotationStep * 3) + 24);
  }

  delay(250);
}
