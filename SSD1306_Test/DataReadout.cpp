#include "DataReadout.h"

/*
 * Constructor
 */
DataReadout::DataReadout(Adafruit_SSD1306 *readout, DisplayData *data) {
  Serial.println("DataReadout init called!");

  display = readout;
  displayData = data;

  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 Allocation Failed!!!");
    for (;;);
  }

  displayAllItems();

  delay(2000);
}


void DataReadout::updateDisplay() {

  //  if (displayData->muteButtonState == true) {
  //    displayMute();
  //  }
  //  else {
  displayAllItems();
  //  }
}


/*
 * Private
 */
 
void DataReadout::resetText() {
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setFont(NULL);
}


void DataReadout::displayPhantom() {
  resetText();
  display->setCursor(1, 1);

  if (displayData->phantomButtonState == HIGH) {
    display->println("48");
  }
}


void DataReadout::displayPolarity() {
  resetText();

  if (displayData->polarityButtonState == HIGH) {
    display->setCursor(41, 1);
    display->println("180");
  }
}


void DataReadout::displayInputZ() {
  resetText();

  display->setCursor(92, 1);
  display->println(displayData->inputZButtonState == LOW ? "LOW" : "HI");
}


void DataReadout::displayPad() {
  resetText();

  if (displayData->padButtonState == HIGH) {
    display->setCursor(92, 26);
    display->println("PAD");
  }
}


void DataReadout::displayHPF() {
  resetText();

  if (displayData->highPassFilterButtonState == HIGH) {
    display->setCursor(92, 50);
    display->println("HPF");
  }
}


void DataReadout::displayGain() {    
  display->setTextSize(3);
  display->setCursor(1, 35);
  display->println("+");

  display->setTextSize(5);
  display->setCursor(20, 28);
  display->println(displayData->gainLevel);
}


void DataReadout::displayAllItems() {
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


void DataReadout::displayMute() {
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
