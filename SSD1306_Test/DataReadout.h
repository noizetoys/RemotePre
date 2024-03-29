#ifndef DATAREADOUT_H
#define DATAREADOUT_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MicPreData.h"


class DataReadout {
  private:
    Adafruit_SSD1306 *display;

    void resetText();
    
    void displayAllItems();
    void displayMute();
    void displayID();

    void displayPhantom();
    void displayPolarity();
    void displayInputZ();
    void displayPad();
    void displayHPF();
    void displayGain();

    MicPreData *micPreData;


  public:
    void updateDisplay();

    DataReadout() {}
    DataReadout(Adafruit_SSD1306 *readout, MicPreData *data);
};

#endif
