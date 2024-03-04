
#ifndef MICPRECONTROL_H
#define MICPRECONTROL_H

#include <Arduino.h>
#include "MicPreData.h"
#include "DataReadout.h"
//#include "NetworkInterface.h"


//#define EncoderMin 1
//#define EncoderMax 120
//#define EncoderStep 8



class MicPreControl {
  private:
    MicPreData *micPreData;
//    DataReadout *dataReadout;
    //    NetworkInterface network;

    bool dataChanged;
//    volatile int encoder0Pos;
//    volatile int rotationStep, newRotationStep, btn;

    void updateState(bool newPinState, bool *buttonMode, bool *state);


  public:
    void updateGainLevel(int AValue, int BValue);

    void updateMute(bool newPinState);

    void updatePad(bool newPinState);
    void updatePhantom(bool newPinState);
    void updatePolarity(bool newPinState);
    void updateInputZ(bool newPinState);
    void updateHPF(bool newPinState);

//    void setInfo(MicPreData *data, DataReadout *readoutDisplay);

    // Constructors
    MicPreControl() { }
        MicPreControl(MicPreData *data);

    //    MicPreControl(int id, MicPreData *data, DataReadout *readoutDisplay);
//    MicPreControl(MicPreData *data, DataReadout *readoutDisplay);
};

#endif
