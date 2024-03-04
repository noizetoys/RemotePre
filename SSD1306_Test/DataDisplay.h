#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MicPreData.h"


class DataDisplay {
  private:
    Adafruit_SSD1306 *display;

    void resetText();
    void displayAllItems();
    void displayMute();

    void displayPhantom();
    void displayPolarity();
    void displayInputZ();
    void displayPad();
    void displayHPF();
    void displayGain();

    MicPreData *micPreData;

  public:
    void updateDisplay();

    DataDisplay() {}
    DataDisplay(Adafruit_SSD1306 *readout, MicPreData *data);
};

#endif
