// ControlArduino.ino
#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"
#include "DataDisplay.h"
#include "Config.h"
#include "Logger.h"
#include "StateManager.h"

Rotary rotary(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B);
StateManager stateManager;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DataDisplay dataDisplay(display);

uint8_t i2cAddress;
bool communicationLost = false;

void setup() {
  // ... (setup code remains the same)
}

void loop() {
  checkAndSignalStateChanges();
  debounceButtons();
  
  if (communicationLost) {
    dataDisplay.displayCommunicationLost();
  } else {
    dataDisplay.updateDisplay(stateManager.getControlState());
  }
}

// ... (interrupt handlers and other functions remain the same)

void receiveControlStates(int byteCount) {
  if (Wire.available() >= 7) {
    ControlState newState;
    newState.gainLevel = Wire.read();
    newState.phaseState = Wire.read();
    newState.phantomPowerState = Wire.read();
    newState.inputImpedanceState = Wire.read();
    newState.padState = Wire.read();
    newState.highPassFilterState = Wire.read();
    newState.muteState = Wire.read();

    if (newState != stateManager.getControlState()) {
      stateManager.updateControlState(newState);
      dataDisplay.updateDisplay(stateManager.getControlState());
      communicationLost = false;
    }
  }
}