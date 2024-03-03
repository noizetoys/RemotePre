
#include "MicPreControl.h"


MicPreControl::MicPreControl(MicPreData *data, DataReadout *readoutDisplay)  {
  Serial.print("* MicPreControl::MicPreControl() called!");

  micPreData = data;
  dataReadout = readoutDisplay;
  //  network = NetworkInterface(id);
}


/*
   Public
*/

void MicPreControl::updateGainLevel(int AValue, int BValue) {
  Serial.println("MicPreControl: Phantom Button Pressed");
    Serial.print("MicPreControl::updateGainLevel:  Current Encoder Value = ");
  //  Serial.println(encoder0Pos);
  
    if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
      AValue == BValue ? encoder0Pos++ : encoder0Pos--;
    }

  newRotationStep = (encoder0Pos / EncoderStep);

  if (newRotationStep != rotationStep && encoder0Pos >= EncoderStep) {
    rotationStep = newRotationStep;
    Serial.print("\nRotationStep = ");
    Serial.println(rotationStep);
  }

  if (encoder0Pos < EncoderMin) {
    Serial.print(EncoderMin);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
    Serial.print(EncoderMax);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

//  micPreData->gainLevel = rotationStep;

  if (micPreData->gainLevel != rotationStep) {
    Serial.println("MicPreControl: Pad Button Pressed");
    micPreData->gainLevel = rotationStep;

    /*
       Send Data to Network for re-sending to Pre
    */
  }

  dataReadout->updateDisplay();
}






void MicPreControl::updatePad(bool newPinState) {
  if (micPreData->padButtonState != newPinState) {
    Serial.println("MicPreControl: Pad Button Pressed");
    updateState(newPinState, &micPreData->padButtonState, &micPreData->padEngaged);
  }
}


void MicPreControl::updatePhantom(bool newPinState) {
  if (micPreData->phantomButtonState != newPinState) {
    Serial.println("MicPreControl: Phantom Button Pressed");
    updateState(newPinState, &micPreData->phantomButtonState, &micPreData->phantomEngaged);
  }
}


void MicPreControl::updatePolarity(bool newPinState) {
  if (micPreData->polarityButtonState != newPinState) {
    Serial.println("MicPreControl: Polarity Button Pressed");
    updateState(newPinState, &micPreData->polarityButtonState, &micPreData->polarityInverted);
  }
}


void MicPreControl::updateInputZ(bool newPinState) {
  if (micPreData->inputZButtonState != newPinState) {
    Serial.println("MicPreControl: InputZ Button Pressed");
    updateState(newPinState, &micPreData->inputZButtonState, &micPreData->inputZIsHigh);
  }
}


void MicPreControl::updateMute(bool newPinState) {
  if (micPreData->muteButtonState != newPinState) {
    Serial.println("MicPreControl: Mute Button Pressed");
    updateState(newPinState, &micPreData->muteButtonState, &micPreData->muteEngaged);
  }
}


void MicPreControl::updateHPF(bool newPinState) {
  if (micPreData->highPassFilterButtonState != newPinState) {
    Serial.println("MicPreControl: High Pass Filter Button Pressed");
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
void MicPreControl::updateState(bool newPinState, bool *buttonMode, bool *state) {
  Serial.print("\n--> updateState:  ");
  Serial.print("newPinState: ");
  Serial.print(newPinState);
  Serial.print("\tbuttonMode: ");
  Serial.print(*buttonMode);
  Serial.print("\tstate: ");
  Serial.println(*state);

  if ((newPinState == true) && (*buttonMode == false)) {
    *buttonMode = true;
    Serial.println("Button has been pressed.");
  }
  else if ((newPinState == false) && (*buttonMode == true)) {
    *buttonMode = false;
    Serial.println("Button has been released");
    *state = *state == true ? false : true;

    Serial.print("Thing Is On? ");
    Serial.println(*state == true ? "YES" : "NO");
  }

  /*
    Send Data to Network for re-sending to Pre
  */
}
