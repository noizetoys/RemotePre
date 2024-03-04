
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
    Serial.print("* MicPreData(int id) called!  ID = ");
    Serial.println(id);

    deviceID = id;
    gainLevel = 1;

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

  void serialPrintData() {
    //  if (muteButtonState == LOW) {
    //    //  if (digitalRead(MUTE_BUTTON) == HIGH || digitalRead(encoder0Btn) == HIGH) {
    //    Serial.println("MUTE should be Shown!");
    //  }

    // Mute
    Serial.print("\nEncoder Button State:  ");
    Serial.println(muteButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("Mute Engaged:  ");
    Serial.println(muteEngaged == HIGH ? "YES" : "NO");

    // Phantom
    Serial.print("Phantom State:  ");
    Serial.println(phantomButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("Phantom Engaged:  ");
    Serial.println(phantomEngaged == HIGH ? "YES" : "NO");

    // Polarity
    Serial.print("Polarity State:  ");
    Serial.println(polarityButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("Polarity Inverted:  ");
    Serial.println(polarityInverted == HIGH ? "YES" : "NO");

    // Input Z
    Serial.print("Input Z State:  ");
    Serial.println(inputZButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("Mute Engaged:  ");
    Serial.println(inputZIsHigh == HIGH ? "YES" : "NO");

    // Pad
    Serial.print("Pad State:  ");
    Serial.println(padButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("Pad Engaged:  ");
    Serial.println(padEngaged == HIGH ? "YES" : "NO");

    // High Pass Filter
    Serial.print("HPF State:  ");
    Serial.println(highPassFilterButtonState == HIGH ? "HIGH" : "LOW");
    Serial.print("HPF Engaged:  ");
    Serial.println(highPassFilterEngaged == HIGH ? "YES" : "NO");

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
