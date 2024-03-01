
#ifndef MicPreController_h
#define MicPreController_h

#include "DataDisplay.h"
#include "NetworkInterface.h"


class MicPreController {
  private:
    MicPreData data;
    DataDisplay dataDisplay;
    NetworkInterface network;

    void updateState(bool newPinState, bool *buttonMode, bool *state);

  public:
    void updateGainLevel(int value);
    void updatePad(bool newPinState);
    void updatePhantom(bool newPinState);
    void updatePolarity(bool newPinState);
    void updateInputZ(bool newPinState);
    void updateMute(bool newPinState);
    void updateHPF(bool newPinState);

    // Constructors
    MicPreController() { }

    // Non-Default
    MicPreController(char id);
};

#endif
