
#include "MicPreStateProcessor.h"
#include "HardwareSerial.h"


MicPreStateProcessor::MicPreStateProcessor(MicPreData *data)  {
//  Serial.println(F("* MicPreControl: init called!"));
  micPreData = data;
}


// Public

void MicPreStateProcessor::updateGainLevel(int AValue, int BValue) {
//  Serial.print(F("~~ MicPreControl: updateGainLevel() called!"));
//  Serial.print(F(" --> AValue: "));
//  Serial.print(AValue);
//  Serial.print(F(", BValue: "));
//  Serial.println(BValue);

  //  Serial.print(F("MicPreStateProcessor::updateGainLevel:  Current Encoder Value = "));

  if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
    AValue == BValue ? encoder0Pos++ : encoder0Pos--;
  }

  newRotationStep = (encoder0Pos / EncoderStep);

  if (newRotationStep != rotationStep && encoder0Pos >= EncoderStep) {
    rotationStep = newRotationStep;
//    Serial.print(F("\nRotationStep = "));
//    Serial.println(rotationStep);
  }

  if (encoder0Pos < EncoderMin) {
//    Serial.print(EncoderMin);
//    Serial.println(F(" Hit, No Change"));
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
//    Serial.print(EncoderMax);
//    Serial.println(F(" Hit, No Change"));
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

  if (micPreData->gainLevel != rotationStep) {
//    Serial.print(F("Gain Level Changed:  "));
//    Serial.println(rotationStep);
    micPreData->gainLevel = rotationStep;
  }
}


void MicPreStateProcessor::updatePad(bool newPinState) {
  if (micPreData->padButtonState != newPinState) {
    Serial.println(F("MicPreControl: Pad Button Pressed"));
    updateState(newPinState, &micPreData->padButtonState, &micPreData->padEngaged);
  }
}


void MicPreStateProcessor::updatePhantom(bool newPinState) {
  if (micPreData->phantomButtonState != newPinState) {
    Serial.println(F("MicPreControl: Phantom Button Pressed"));
    updateState(newPinState, &micPreData->phantomButtonState, &micPreData->phantomEngaged);
  }
}


void MicPreStateProcessor::updatePolarity(bool newPinState) {
  if (micPreData->polarityButtonState != newPinState) {
    Serial.println(F("MicPreControl: Polarity Button Pressed"));
    updateState(newPinState, &micPreData->polarityButtonState, &micPreData->polarityInverted);
  }
}


void MicPreStateProcessor::updateInputZ(bool newPinState) {
  if (micPreData->inputZButtonState != newPinState) {
    Serial.println(F("MicPreControl: InputZ Button Pressed"));
    updateState(newPinState, &micPreData->inputZButtonState, &micPreData->inputZIsHigh);
  }
}


void MicPreStateProcessor::updateMute(bool newPinState) {
  if (micPreData->muteButtonState != newPinState) {
    Serial.println(F("\nMicPreControl: Mute Button Pressed"));
    Serial.print(F("muteButtonState = "));
    Serial.print(micPreData->muteButtonState);
    Serial.print(F(", newPinState = "));
    Serial.println(newPinState);
    updateState(newPinState, &micPreData->muteButtonState, &micPreData->muteEngaged);
  }
}


void MicPreStateProcessor::updateHPF(bool newPinState) {
  if (micPreData->highPassFilterButtonState != newPinState) {
    Serial.println(F("MicPreControl: High Pass Filter Button Pressed"));
    updateState(newPinState, &micPreData->highPassFilterButtonState, &micPreData->highPassFilterEngaged);
  }
}


/*
   Pin State (Pressed/Released)

   Function Button Mode (Pressed/Released)
   Function Button State (On/Off)

   Function State (active/inactive)
*/

// Private
void MicPreStateProcessor::updateState(bool newPinState, bool *buttonMode, bool *state) {
//  Serial.print(F("\n--> updateState:  "));
//  Serial.print(F("newPinState: "));
//  Serial.print(newPinState);
//  Serial.print(F("\tbuttonMode: "));
//  Serial.print(*buttonMode);
//  Serial.print(F("\tstate: "));
//  Serial.println(*state);

  if ((newPinState == true) && (*buttonMode == false)) {
    *buttonMode = true;
    Serial.println(F("Button has been pressed."));
  }
  else if ((newPinState == false) && (*buttonMode == true)) {
    *buttonMode = false;
    *state = *state == true ? false : true;
    Serial.println(F("Button has been released"));

    Serial.print(F("Thing Is On? "));
    Serial.println(*state == true ? "YES" : "NO");
  }

  /*
    Send Data to Network for re-sending to Pre
  */
}
