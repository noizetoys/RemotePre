#ifndef MICPRESTATEPROCESSOR_H
#define MICPRESTATEPROCESSOR_H

#include <Arduino.h>
#include "MicPreData.h"
//#include "NetworkInterface.h"


class MicPreStateProcessor {
  private:
    MicPreData *micPreData;
    //    NetworkInterface network;

    int EncoderMin = 1;
    int EncoderMax = 120;
    int EncoderStep = 8;

    volatile int encoder0Pos = 0;
    volatile int AValue;
    volatile int BValue;
    volatile int rotationStep, newRotationStep, btn;

    bool dataChanged;

    void updateState(bool newPinState, bool *buttonMode, bool *state);


  public:
    void updateGainLevel(int AValue, int BValue);

    void updateMute(bool newPinState);

    void updatePad(bool newPinState);
    void updatePhantom(bool newPinState);
    void updatePolarity(bool newPinState);
    void updateInputZ(bool newPinState);
    void updateHPF(bool newPinState);

    // Constructors
    MicPreStateProcessor() { }
    MicPreStateProcessor(MicPreData *data);
};

#endif
