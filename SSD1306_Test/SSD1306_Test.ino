#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int buttonState;
#define MUTE_BUTTON 8

#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
volatile int encoder0Pos = 0;


void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 Allocation Failed!!!");
    for (;;);
  }

  delay(2000);

  pinMode(MUTE_BUTTON, INPUT);
  // Encoder
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);

  attachInterrupt(0, doEncoder, CHANGE);

  displayMute();
}

int encoderButtonState;

void resetText() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);
}


void displayPhantom() {
  resetText();
  display.setCursor(1, 1);
  if (buttonState == HIGH) {
    display.println("48v");
  }
}


void displayPolarity() {
  resetText();
  if (buttonState == HIGH) {
    display.setCursor(41, 1);
    display.println("180");
  }
}


void displayInputZ() {
  resetText();
  display.setCursor(92, 1);
  display.println(buttonState == LOW ? "LOW" : "HI");
}


void displayPad() {
  resetText();
  if (buttonState == HIGH) {
    display.setCursor(92, 26);
    display.println("PAD");
  }
}


void displayHPF() {
  resetText();
  if (buttonState == HIGH) {
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


volatile int AValue;
volatile int BValue;

#define EncoderMin 1
#define EncoderMax 120
#define EncoderStep 8

volatile int rotationStep, newRotationStep, btn;


void doEncoder() {
  AValue = digitalRead(encoder0PinA);
  BValue = digitalRead(encoder0PinB);

  if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
    AValue == BValue ? encoder0Pos++ : encoder0Pos--;
  }

  Serial.print("doEncoder:  Current Encoder Value = ");
  Serial.println(encoder0Pos);

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



void loop() {
  encoderButtonState = digitalRead(encoder0Btn);
  buttonState = digitalRead(MUTE_BUTTON);

  //  Serial.print("Encoder Button State:  ");
  //  Serial.print(encoderButtonState == HIGH ? "HIGH" : "LOW");
  //  Serial.print("Button State:  ");
  //  Serial.println(buttonState == HIGH ? "HIGH" : "LOW");

  if (digitalRead(encoder0Btn) == HIGH) {
    //  if (digitalRead(MUTE_BUTTON) == HIGH || digitalRead(encoder0Btn) == HIGH) {
    displayMute();
    Serial.println("MUTE should be Shown!");
  }
  else {
    displayAllItems((rotationStep * 3) + 24);
  }

  delay(50);
}
