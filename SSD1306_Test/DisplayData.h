
#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <HardwareSerial.h>

typedef struct DisplayData {
  unsigned int gainLevel;

  bool padButtonState;
  bool phantomButtonState;
  bool polarityButtonState;
  bool inputZButtonState;
  bool muteButtonState;
  bool highPassFilterButtonState;

  //  DisplayData::DisplayData() {
  DisplayData() {
    gainLevel = 1;

    padButtonState = false;
    phantomButtonState = false;
    polarityButtonState = false;
    inputZButtonState = false;
    muteButtonState = false;
    highPassFilterButtonState = false;
  }

  //  DisplayData createFromData(DisplayData data) {
  //        Serial.println("MicPreData.resetData() called.");
  //
  //    DisplayData data = DisplayData();
  //
  //    data.padButtonState = padButtonState;
  //    data.phantomButtonState = phantomButtonState;
  //    data.polarityButtonState = polarityButtonState;
  //    data.inputZButtonState = inputZButtonState;
  //    data.muteButtonState = muteButtonState;
  //    data.highPassFilterButtonState = highPassFilterButtonState;
  //
  //    return data;
  //  }

} DisplayData;

#endif
