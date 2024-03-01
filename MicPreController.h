
#ifndef MicPreController_h
#define MicPreController_h

#include "DataDisplay.h"
#include "NetworkInterface.h"

#define PRESSED 1
#define RELEASED 0
#define ON 1
#define OFF 0


class MicPreController {
  private:
    MicPreData data;
    DataDisplay dataDisplay;
    NetworkInterface network;

    // For Buttons
    bool padButtonState;
    bool phantomButtonState;
    bool polarityButtonState;
    bool inputZButtonState;
    bool muteButtonState;
    bool highPassFilterButtonState;

    void updateState(bool newPinState, bool *buttonMode, bool *state);

  public:
    void updateGainLevel(int value);
    void updatePad(bool newPinState);
    void updatePhantom(bool newPinState);
    void updatePolarity(bool newPinState);
    void updateInputZ(int newPinState);
    void updateMute(int newPinState);
    void updateHPF(int newPinState);

    // Constructors
    // Default
    MicPreController() { }

    // Non-Default
    MicPreController(char id);
};

#endif
