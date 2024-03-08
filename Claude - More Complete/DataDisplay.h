// DataDisplay.h
#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"

class DataDisplay {
public:
  DataDisplay(Adafruit_SSD1306& display);
  void init();
  void updateDisplay(const ControlState& state);
  void displayCommunicationLost();

private:
  Adafruit_SSD1306& display;

  void resetText();
  void displayPhantom(bool state);
  void displayPolarity(bool state);
  void displayInputZ(bool state);
  void displayPad(bool state);
  void displayHPF(bool state);
  void displayGain(int level);
  void displayMute(bool state);
  void displayAllItems(const ControlState& state);
};