
#ifndef MICPREDATA_H
#define MICPREDATA_H

#include <HardwareSerial.h>

typedef struct MicPreData {
  unsigned int gainLevel;

  bool padEngaged;
  bool phantomEngaged;
  bool polarityInverted;
  bool inputZIsHigh;
  bool muteEngaged;
  bool highPassFilterEngaged;

  // Needed to keep local and remote interfaces in sync
  bool padButtonState;
  bool phantomButtonState;
  bool polarityButtonState;
  bool inputZButtonState;
  bool muteButtonState;
  bool highPassFilterButtonState;

  unsigned int deviceID;

  MicPreData() { }

  MicPreData(int id) {
    Serial.println("* MicPreData() called.");

    deviceID = id;
  }

  void resetData() {
    Serial.println("MicPreData.resetData() called.");

    padEngaged = false;
    phantomEngaged = false;
    polarityInverted = false;
    inputZIsHigh = false;
    muteEngaged = false;
    highPassFilterEngaged = false;

    padButtonState = false;
    phantomButtonState = false;
    polarityButtonState = false;
    inputZButtonState = false;
    muteButtonState = false;
    highPassFilterButtonState = false;
  }

  MicPreData copy() {
    Serial.println("MicPreData.copy() called.");

    MicPreData data = MicPreData();

    data.padEngaged = padEngaged;
    data.phantomEngaged = phantomEngaged;
    data.polarityInverted = polarityInverted;
    data.inputZIsHigh = inputZIsHigh;
    data.muteEngaged = muteEngaged;
    data.highPassFilterEngaged = highPassFilterEngaged;

    data.padButtonState = padButtonState;
    data.phantomButtonState = phantomButtonState;
    data.polarityButtonState = polarityButtonState;
    data.inputZButtonState = inputZButtonState;
    data.muteButtonState = muteButtonState;
    data.highPassFilterButtonState = highPassFilterButtonState;
  }

} MicPreData;

#endif
