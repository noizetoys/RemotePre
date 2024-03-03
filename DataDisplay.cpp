

#include "DataDisplay.h"
//#include "Arduino.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


DataDisplay::DataDisplay() {
  Serial.println("* DataDisplay::DataDisplay() called");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 Allocation Failed!!!");
    for (;;);
  }

  Serial.println("DataDisplay() : SSD1306 Allocated!!!");
}


void DataDisplay::displayData(MicPreData data) {
  Serial.print("DataDisplay::displayData(MicPreData data) ->");
//  Serial.println(data);

  if (data.muteEngaged) {
    displayMute();
  }
  else {
    displayReadout(data);
  }
}


void DataDisplay::displayMute() {
  display.clearDisplay();
  display.invertDisplay(true);

  display.drawRect(0, 0, 128, 16, WHITE);

  display.setTextColor(WHITE);
  display.setTextSize(5);
  display.setFont(NULL);

  display.setCursor(7, 23);
  display.println("MUTE");

  display.display();
}


void DataDisplay::displayReadout(MicPreData data) {
  display.clearDisplay();
  display.invertDisplay(false);

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);

  if (data.phantomEngaged) {
    display.setCursor(1, 1);
    display.println("48");
  }

  if (data.polarityInverted) {
    display.setCursor(41, 1);
    display.println("POL");
  }

  display.setCursor(92, 1);
  display.println(data.inputZIsHigh == true ? "HI" : "LOW");

  display.setFont(NULL);

  display.setTextSize(3);
  display.setCursor(1, 35);
  display.println("+");

  display.setTextSize(5);
  display.setCursor(20, 28);
  display.println("35");

  display.setFont(NULL);
  display.setTextSize(2);

  if (data.padEngaged) {
    display.setCursor(92, 26);
    display.println("PAD");
  }

  if (data.highPassFilterEngaged) {
    display.setCursor(92, 50);
    display.println("HPF");
  }

  display.display();
}
