// ControlArduino.ino
#include <SPI.h>
#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"
#include "DataDisplay.h"
#include "Config.h"
#include "Logger.h"
#include "StateManager.h"

// Define SPI pins for OLED display
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 8

Rotary rotary(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B);
StateManager stateManager;
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);
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

// Rest of the code remains the same...

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
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    int newGainState = (stateManager.getControlState().gainLevel + 1) % NUM_GAIN_VALUES;
    stateManager.updateGainState(newGainState);
    dataDisplay.updateDisplay(stateManager.getControlState());
  } else if (result == DIR_CCW) {
    int newGainState = (stateManager.getControlState().gainLevel - 1 + NUM_GAIN_VALUES) % NUM_GAIN_VALUES;
    stateManager.updateGainState(newGainState);
    dataDisplay.updateDisplay(stateManager.getControlState());
  }
}

void handlePhase() {
  phaseButtonState = BUTTON_PRESSED;
}

void handlePhantomPower() {
  phantomPowerButtonState = BUTTON_PRESSED;
}

void handleInputImpedance() {
  inputImpedanceButtonState = BUTTON_PRESSED;
}

void handlePad() {
  padButtonState = BUTTON_PRESSED;
}

void handleHighPassFilter() {
  highPassFilterButtonState = BUTTON_PRESSED;
}

void handleMute() {
  stateManager.toggleMuteState();
  dataDisplay.updateDisplay(stateManager.getControlState());
}

uint8_t readDIPSwitches() {
  uint8_t address = I2C_BASE_ADDRESS;
  for (uint8_t i = 0; i < NUM_DIP_SWITCH_PINS; i++) {
    int pinValue = digitalRead(DIP_SWITCH_PIN_START + i);
    address |= (pinValue << i);
  }
  return address;
}

void checkAndSignalStateChanges() {
  if (stateManager.isStateChanged()) {
    digitalWrite(MULTIPLEXER_CONTROL_PIN, HIGH);
    delay(10);
    digitalWrite(MULTIPLEXER_CONTROL_PIN, LOW);
    stateManager.resetStateChanged();
  }
}

void debounceButtons() {
  if (phaseButtonState == BUTTON_PRESSED && digitalRead(PHASE_BUTTON_PIN) == LOW) {
    debounceIterations++;
    if (debounceIterations >= DEBOUNCE_ITERATIONS) {
      phaseButtonState = BUTTON_DEBOUNCED;
      stateManager.togglePhaseState();
      dataDisplay.updateDisplay(stateManager.getControlState());
      debounceIterations = 0;
    }
  } else if (phaseButtonState == BUTTON_DEBOUNCED && digitalRead(PHASE_BUTTON_PIN) == HIGH) {
    phaseButtonState = BUTTON_RELEASED;
  }

  if (phantomPowerButtonState == BUTTON_PRESSED && digitalRead(PHANTOM_POWER_BUTTON_PIN) == LOW) {
    debounceIterations++;
    if (debounceIterations >= DEBOUNCE_ITERATIONS) {
      phantomPowerButtonState = BUTTON_DEBOUNCED;
      stateManager.togglePhantomPowerState();
      dataDisplay.updateDisplay(stateManager.getControlState());
      debounceIterations = 0;
    }
  } else if (phantomPowerButtonState == BUTTON_DEBOUNCED && digitalRead(PHANTOM_POWER_BUTTON_PIN) == HIGH) {
    phantomPowerButtonState = BUTTON_RELEASED;
  }

  if (inputImpedanceButtonState == BUTTON_PRESSED && digitalRead(INPUT_IMPEDANCE_BUTTON_PIN) == LOW) {
    debounceIterations++;
    if (debounceIterations >= DEBOUNCE_ITERATIONS) {
      inputImpedanceButtonState = BUTTON_DEBOUNCED;
      stateManager.toggleInputImpedanceState();
      dataDisplay.updateDisplay(stateManager.getControlState());
      debounceIterations = 0;
    }
  } else if (inputImpedanceButtonState == BUTTON_DEBOUNCED && digitalRead(INPUT_IMPEDANCE_BUTTON_PIN) == HIGH) {
    inputImpedanceButtonState = BUTTON_RELEASED;
  }

  if (padButtonState == BUTTON_PRESSED && digitalRead(PAD_BUTTON_PIN) == LOW) {
    debounceIterations++;
    if (debounceIterations >= DEBOUNCE_ITERATIONS) {
      padButtonState = BUTTON_DEBOUNCED;
      stateManager.togglePadState();
      dataDisplay.updateDisplay(stateManager.getControlState());
      debounceIterations = 0;
    }
  } else if (padButtonState == BUTTON_DEBOUNCED && digitalRead(PAD_BUTTON_PIN) == HIGH) {
    padButtonState = BUTTON_RELEASED;
  }

  if (highPassFilterButtonState == BUTTON_PRESSED && digitalRead(HIGH_PASS_FILTER_BUTTON_PIN) == LOW) {
    debounceIterations++;
    if (debounceIterations >= DEBOUNCE_ITERATIONS) {
      highPassFilterButtonState = BUTTON_DEBOUNCED;
      stateManager.toggleHighPassFilterState();
      dataDisplay.updateDisplay(stateManager.getControlState());
      debounceIterations = 0;
    }
  } else if (highPassFilterButtonState == BUTTON_DEBOUNCED && digitalRead(HIGH_PASS_FILTER_BUTTON_PIN) == HIGH) {
    highPassFilterButtonState = BUTTON_RELEASED;
  }
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