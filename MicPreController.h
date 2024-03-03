
#ifndef MICPRECONTROLLER_H
#define MICPRECONTROLLER_H

#include <Arduino.h>
#include "DataDisplay.h"
#include "NetworkInterface.h"


class MicPreController {
  private:
    MicPreData micPreData;
    DataDisplay dataDisplay;
    
    NetworkInterface network;

    void updateState(bool newPinState, bool *buttonMode, bool *state);


  public:
    void updateGainLevel(int value);

    void updateMute(bool newPinState);

    void updatePad(bool newPinState);
    void updatePhantom(bool newPinState);
    void updatePolarity(bool newPinState);
    void updateInputZ(bool newPinState);
    void updateHPF(bool newPinState);

    // Constructors
    MicPreController() { }

    // Non-Default
    MicPreController(char id, Adafruit_SSD1306 *oledDisplay);
};

#endif
