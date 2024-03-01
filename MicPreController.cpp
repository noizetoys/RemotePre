#include <Arduino.h>
#include "MicPreController.h"

MicPreController::MicPreController(char id)  {
  data = MicPreData();
  dataDisplay = DataDisplay();
  network = NetworkInterface(id);
}


//void MicPreController::updateGainLevel(char value) {
//
//}

void MicPreController::updatePad(bool newPinState) {
  if (data.padButtonState != newPinState) {
    updateState(newPinState, &data.padButtonState, &data.padEngaged);
  }
}

void MicPreController::updatePhantom(bool newPinState) {
  if (data.phantomButtonState != newPinState) {
    updateState(newPinState, &data.phantomButtonState, &data.phantomEngaged);
  }
}

void MicPreController::updatePolarity(bool newPinState) {
  if (data.polarityButtonState != newPinState) {
    updateState(newPinState, &data.polarityButtonState, &data.polarityInverted);
  }
}

//void MicPreController::updateInputZ(char newPinState) {
//
//}
//
//void MicPreController::updateMute(char newPinState) {
//
//}
//
//void MicPreController::updateHPF(char newPinState) {
//
//}


// Private
void MicPreController::updateState(bool newPinState, bool *buttonMode, bool *state) {
  Serial.print("\n--> updateState:");
  Serial.print("\tnewPinState: ");
  Serial.print(newPinState);
  Serial.print("\tbuttonMode: ");
  Serial.print(*buttonMode);
  Serial.print("\tstate: ");
  Serial.println(*state);

  //  if ((newPinState == PRESSED) && (*buttonMode == RELEASED)) {
  //    *buttonMode = PRESSED;
  if ((newPinState == true) && (*buttonMode == false)) {
    *buttonMode = true;
    Serial.println("\nButton has been pressed.");
  }
  //  else if ((newPinState == RELEASED) && (*buttonMode == PRESSED)) {
  //    *buttonMode = RELEASED;
  //    Serial.println("\nButton has been released");
  //    *state = *state == true ? false : true;
  else if ((newPinState == false) && (*buttonMode == true)) {
    *buttonMode = false;
    Serial.println("\nButton has been released");
    *state = *state == true ? false : true;

    Serial.print("Thing Is On? ");
    Serial.println(*state == true ? "YES" : "NO");
  }
}
