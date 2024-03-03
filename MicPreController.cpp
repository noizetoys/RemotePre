
#include "MicPreController.h"


MicPreController::MicPreController(char id, Adafruit_SSD1306 *oledDisplay)  {
  Serial.print("* MicPreController::MicPreController(char id) called!  ID = ");
  Serial.println(int(id));

  micPreData = MicPreData();
  dataDisplay = DataDisplay(oledDisplay, &micPreData);
  //  network = NetworkInterface(id);

  /*
      Required to set all data properties to their 'OFF' state
      Otherwise the properties contain garbage
  */
  micPreData.resetData();
}


void MicPreController::updateGainLevel(int value) {
    Serial.println("MicPreController: Phantom Button Pressed");

}

void MicPreController::updatePad(bool newPinState) {
  if (micPreData.padButtonState != newPinState) {
    Serial.println("MicPreController: Pad Button Pressed");
    updateState(newPinState, &micPreData.padButtonState, &micPreData.padEngaged);
  }
}

void MicPreController::updatePhantom(bool newPinState) {
  if (micPreData.phantomButtonState != newPinState) {
    Serial.println("MicPreController: Phantom Button Pressed");
    updateState(newPinState, &micPreData.phantomButtonState, &micPreData.phantomEngaged);
  }
}

void MicPreController::updatePolarity(bool newPinState) {
  if (micPreData.polarityButtonState != newPinState) {
    Serial.println("MicPreController: Polarity Button Pressed");
    updateState(newPinState, &micPreData.polarityButtonState, &micPreData.polarityInverted);
  }
}

void MicPreController::updateInputZ(bool newPinState) {
  if (micPreData.inputZButtonState != newPinState) {
    Serial.println("MicPreController: InputZ Button Pressed");
    updateState(newPinState, &micPreData.inputZButtonState, &micPreData.inputZIsHigh);
  }
}

void MicPreController::updateMute(bool newPinState) {
  if (micPreData.muteButtonState != newPinState) {
    Serial.println("MicPreController: Mute Button Pressed");
    updateState(newPinState, &micPreData.muteButtonState, &micPreData.muteEngaged);
  }
}

void MicPreController::updateHPF(bool newPinState) {
  if (micPreData.highPassFilterButtonState != newPinState) {
    Serial.println("MicPreController: High Pass Filter Button Pressed");
    updateState(newPinState, &micPreData.highPassFilterButtonState, &micPreData.highPassFilterEngaged);
  }
}


/*
   Pin State (Pressed/Released)

   Function Button Mode (Pressed/Released)
   Function Button State (On/Off)

   Function State (active/inactive)
*/

// Private
void MicPreController::updateState(bool newPinState, bool *buttonMode, bool *state) {
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
}
