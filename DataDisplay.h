
#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include "MicPreData.h"
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


    //['25', '29', '33', '36', '39', '42', '45', '48', '51', '54', '57', '60'];

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
