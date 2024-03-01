#include <Arduino.h>
#include "MicPreController.h"

MicPreController::MicPreController(char id)  {
  data = MicPreData();
  dataDisplay = DataDisplay();
  network = NetworkInterface(id);

  data.resetData();
}


void MicPreController::updateGainLevel(int value) {

}

void MicPreController::updatePad(bool newPinState) {
  //  if (data.padButtonState != newPinState) {
  //    //  if (data.padButtonState != newPinState) {
  //    Serial.print("\n--> MicPreController::updatePad:");
  //    Serial.print(", newPinState: ");
  //    Serial.print(newPinState);
  //
  //    Serial.print(", padButtonState: ");
  //    Serial.print(data.padButtonState);
  //    Serial.print(", padButtonState Address: ");
  //    Serial.print(int(&data.padButtonState));
  //
  //    Serial.print(", padEngaged: ");
  //    Serial.print(data.padEngaged);
  //    Serial.print(", padEngaged Address: ");
  //    Serial.println(int(&data.padEngaged));

  if (data.padButtonState != newPinState) {
    Serial.println("Pad Button Pressed");
    updateState(newPinState, &data.padButtonState, &data.padEngaged);
  }
}

void MicPreController::updatePhantom(bool newPinState) {
  if (data.phantomButtonState != newPinState) {
   Serial.println("Phantom Button Pressed");
   updateState(newPinState, &data.phantomButtonState, &data.phantomEngaged);
  }
}

void MicPreController::updatePolarity(bool newPinState) {
  if (data.polarityButtonState != newPinState) {
    Serial.println("Polarity Button Pressed");
    updateState(newPinState, &data.polarityButtonState, &data.polarityInverted);
  }
}

void MicPreController::updateInputZ(bool newPinState) {
  if (data.inputZButtonState != newPinState) {
    Serial.println("InputZ Button Pressed");
    updateState(newPinState, &data.inputZButtonState, &data.inputZIsHigh);
  }
}

void MicPreController::updateMute(bool newPinState) {
  if (data.muteButtonState != newPinState) {
    Serial.println("Mute Button Pressed");
    updateState(newPinState, &data.muteButtonState, &data.muteEngaged);
  }
}

void MicPreController::updateHPF(bool newPinState) {
  if (data.highPassFilterButtonState != newPinState) {
    Serial.println("High Pass Filter Button Pressed");
    updateState(newPinState, &data.highPassFilterButtonState, &data.highPassFilterEngaged);
  }
}


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
