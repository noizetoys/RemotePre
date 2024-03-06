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
  Logger::init();
  Logger::info("Control Arduino starting...");

  i2cAddress = readDIPSwitches();
  Wire.begin(i2cAddress);
  Logger::debug("I2C address set");

  pinMode(ROTARY_ENCODER_SWITCH_PIN, INPUT_PULLUP);
  pinMode(PHASE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PHANTOM_POWER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INPUT_IMPEDANCE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PAD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(HIGH_PASS_FILTER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MUTE_BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_A), handleRotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_B), handleRotaryEncoder, CHANGE);

  Wire.onReceive(receiveControlStates);

  dataDisplay.init();

  Logger::info("Control Arduino setup completed");
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

void handleRotaryEncoder() {
  // ... (rotary encoder handling code remains the same)
}

void handlePhase() {
  // ... (phase button handling code remains the same)
}

void handlePhantomPower() {
  // ... (phantom power button handling code remains the same)
}

void handleInputImpedance() {
  // ... (input impedance button handling code remains the same)
}

void handlePad() {
  // ... (pad button handling code remains the same)
}

void handleHighPassFilter() {
  // ... (high pass filter button handling code remains the same)
}

void handleMute() {
  stateManager.toggleMuteState();
  dataDisplay.updateDisplay(stateManager.getControlState());
}

uint8_t readDIPSwitches() {
  // ... (DIP switch reading code remains the same)
}

void checkAndSignalStateChanges() {
  // ... (state change signaling code remains the same)
}

void debounceButtons() {
  // ... (button debouncing code remains the same)
}

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