#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <Wire.h>
#include <SPI.h>
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

    int gainValues[12] = { 25, 29, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60 };


  public:
    void updateDisplay();
    void displayDeviceID(int id);

    DataDisplay() {}
    DataDisplay(Adafruit_SSD1306 *readout, MicPreData *data);
};

#endif
