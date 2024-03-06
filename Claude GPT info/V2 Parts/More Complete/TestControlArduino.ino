// TestControlArduino.ino
#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"
#include "DataDisplay.h"
#include "Config.h"
#include "Logger.h"
#include "StateManager.h"

StateManager stateManager;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DataDisplay dataDisplay(display);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  dataDisplay.init();
}

void loop() {
  testRotaryEncoder();
  testPhaseButton();
  testPhantomPowerButton();
  testInputImpedanceButton();
  testPadButton();
  testHighPassFilterButton();
  testMuteButton();
  testCommunicationLost();
  delay(1000);
}

void testRotaryEncoder() {
  Serial.println("Testing rotary encoder...");
  stateManager.updateGainState(50);
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.updateGainState(100);
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testPhaseButton() {
  Serial.println("Testing phase button...");
  stateManager.togglePhaseState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.togglePhaseState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testPhantomPowerButton() {
  Serial.println("Testing phantom power button...");
  stateManager.togglePhantomPowerState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.togglePhantomPowerState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testInputImpedanceButton() {
  Serial.println("Testing input impedance button...");
  stateManager.toggleInputImpedanceState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.toggleInputImpedanceState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testPadButton() {
  Serial.println("Testing pad button...");
  stateManager.togglePadState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.togglePadState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testHighPassFilterButton() {
  Serial.println("Testing high pass filter button...");
  stateManager.toggleHighPassFilterState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.toggleHighPassFilterState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testMuteButton() {
  Serial.println("Testing mute button...");
  stateManager.toggleMuteState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
  stateManager.toggleMuteState();
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}

void testCommunicationLost() {
  Serial.println("Testing communication lost...");
  dataDisplay.displayCommunicationLost();
  delay(1000);
  dataDisplay.updateDisplay(stateManager.getControlState());
  delay(1000);
}