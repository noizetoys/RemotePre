// DataDisplay.cpp
#include "DataDisplay.h"

DataDisplay::DataDisplay(Adafruit_SSD1306& display) : display(display) {}

void DataDisplay::init() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

// Rest of the code remains the same...

void DataDisplay::updateDisplay(const ControlState& state) {
  displayAllItems(state);
}

void DataDisplay::displayCommunicationLost() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("Communication");
  display.println("Lost");
  display.display();
}

void DataDisplay::resetText() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
}

void DataDisplay::displayPhantom(bool state) {
  resetText();
  display.setCursor(1, 1);
  display.println(state ? "48V" : "");
}

void DataDisplay::displayPolarity(bool state) {
  resetText();
  display.setCursor(1, 20);
  display.println(state ? "INVERTED" : "NORMAL");
}

void DataDisplay::displayInputZ(bool state) {
  resetText();
  display.setCursor(1, 40);
  display.println(state ? "HI-Z" : "LOW-Z");
}

void DataDisplay::displayPad(bool state) {
  resetText();
  display.setCursor(70, 1);
  display.println(state ? "PAD" : "");
}

void DataDisplay::displayHPF(bool state) {
  resetText();
  display.setCursor(70, 20);
  display.println(state ? "HPF" : "");
}

void DataDisplay::displayGain(int level) {
  resetText();
  display.setCursor(70, 40);
  display.print("GAIN: ");
  display.println(GAIN_VALUES[level]);
}

void DataDisplay::displayMute(bool state) {
  resetText();
  display.setCursor(1, 52);
  display.println(state ? "MUTE" : "");
}

void DataDisplay::displayAllItems(const ControlState& state) {
  display.clearDisplay();
  displayPhantom(state.phantomPowerState);
  displayPolarity(state.phaseState);
  displayInputZ(state.inputImpedanceState);
  displayPad(state.padState);
  displayHPF(state.highPassFilterState);
  displayGain(state.gainLevel);
  displayMute(state.muteState);
  display.display();
}