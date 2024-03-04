#include "DataDisplay.h"

/*
   Constructor
*/
DataDisplay::DataDisplay(Adafruit_SSD1306 *readout, MicPreData *data) {
  Serial.print(F("\nDataDisplay init called!  ID = "));
  Serial.println(data->deviceID);

  display = readout;
  micPreData = data;

  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
    Serial.println(F("XXX SSD1306 Allocation Failed XXX"));
    Serial.println(F("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
    for (;;);
  }
  else {
    Serial.println(F("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    Serial.println(F("!!! SSD1306 Allocation Succeded!!!"));
    Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
  }

  displayAllItems();

  delay(2000);
}


void DataDisplay::updateDisplay() {

  if (micPreData->muteButtonState == false) {
    displayMute();
  }
  else {
    displayAllItems();
  }
}


/*
   Private
*/

void DataDisplay::resetText() {
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setFont(NULL);
}


void DataDisplay::displayPhantom() {
  resetText();
  display->setCursor(1, 1);

  if (micPreData->phantomButtonState == HIGH) {
    display->println("48");
  }
}


void DataDisplay::displayPolarity() {
  resetText();

  if (micPreData->polarityButtonState == HIGH) {
    display->setCursor(41, 1);
    display->println("180");
  }
}


void DataDisplay::displayInputZ() {
  resetText();

  display->setCursor(92, 1);
  display->println(micPreData->inputZButtonState == LOW ? "LOW" : "HI");
}


void DataDisplay::displayPad() {
  resetText();

  if (micPreData->padButtonState == HIGH) {
    display->setCursor(92, 26);
    display->println("PAD");
  }
}


void DataDisplay::displayHPF() {
  resetText();

  if (micPreData->highPassFilterButtonState == HIGH) {
    display->setCursor(92, 50);
    display->println("HPF");
  }
}


void DataDisplay::displayGain() {
  display->setTextSize(3);
  display->setCursor(1, 35);
  display->println("+");

  display->setTextSize(5);
  display->setCursor(20, 28);
  display->println(micPreData->gainLevel);
}


void DataDisplay::displayAllItems() {
  display->clearDisplay();
  display->invertDisplay(false);

  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setFont(NULL);

  displayPhantom();

  displayPolarity();

  displayInputZ();

  displayPad();

  displayHPF();

  displayGain();

  display->display();
}


void DataDisplay::displayMute() {
  display->clearDisplay();
  display->invertDisplay(true);

  display->drawRect(0, 0, 128, 16, WHITE);

  display->setTextColor(WHITE);
  display->setTextSize(5);
  display->setFont(NULL);

  display->setCursor(7, 23);
  display->println("MUTE");
  display->display();

  delay(2000);
}
