
#ifndef MICPREDATA_H
#define MICPREDATA_H

#include <HardwareSerial.h>

typedef struct MicPreData {
  unsigned int deviceID;

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


  MicPreData() { }

  MicPreData(int id) {
//    Serial.print(F("* MicPreData(int id) called!  ID = "));
//    Serial.println(id);

    deviceID = id;
    gainLevel = 0;

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

  void print() {
    // ID
    Serial.print(F("\n\nDevice ID:  "));
    Serial.println(deviceID);

    // Gain Level
    Serial.print(F("Gain Level:  "));
    Serial.println(gainLevel);

    // Mute
    Serial.print(F("Encoder Button State:  "));
    Serial.print(muteButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", Mute Engaged:  "));
    Serial.println(muteEngaged == true ? "YES" : "NO");

    // Phantom
    Serial.print(F("Phantom State:  "));
    Serial.print(phantomButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", Phantom Engaged:  "));
    Serial.println(phantomEngaged == true ? "YES" : "NO");

    // Polarity
    Serial.print(F("Polarity State:  "));
    Serial.print(polarityButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", Polarity Inverted:  "));
    Serial.println(polarityInverted == true ? "YES" : "NO");

    // Input Z
    Serial.print(F("Input Z State:  "));
    Serial.print(inputZButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", Mute Engaged:  "));
    Serial.println(inputZIsHigh == true ? "YES" : "NO");

    // Pad
    Serial.print(F("Pad State:  "));
    Serial.print(padButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", Pad Engaged:  "));
    Serial.println(padEngaged == true ? "YES" : "NO");

    // High Pass Filter
    Serial.print(F("HPF State:  "));
    Serial.print(highPassFilterButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print(F(", HPF Engaged:  "));
    Serial.println(highPassFilterEngaged == true ? "YES" : "NO");

    Serial.println(F(""));
  }

  //  MicPreData createFromData(MicPreData data) {
  //        Serial.println("MicPreData.resetData() called.");
  //
  //    MicPreData data = MicPreData();
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

} MicPreData;

#endif
