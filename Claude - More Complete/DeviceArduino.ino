// DeviceArduino.ino
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"
#include "DataDisplay.h"

// Define SPI pins for OLED display
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 8

// Define I2C base address and DIP switch pins
const uint8_t I2C_BASE_ADDRESS = 0x08;
const uint8_t DIP_SWITCH_PIN_START = 2;
const uint8_t NUM_DIP_SWITCH_PINS = 5;

// I2C protocol constants
const uint8_t I2C_START_BYTE = 0xAA;
const uint8_t I2C_END_BYTE = 0xBB;

// OLED display object
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Control state object
ControlState controlState;

// Data display object
DataDisplay dataDisplay(display);

bool localControl = false;

void setup() {
  uint8_t i2cAddress = readDIPSwitches();
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveControlStates);

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

// Rest of the code remains the same...

uint8_t readDIPSwitches() {
  uint8_t address = I2C_BASE_ADDRESS;
  for (uint8_t i = 0; i < NUM_DIP_SWITCH_PINS; i++) {
    pinMode(DIP_SWITCH_PIN_START + i, INPUT_PULLUP);
    address |= (digitalRead(DIP_SWITCH_PIN_START + i) << i);
  }
  return address;
}


void loop() {
  if (localControl) {
    handleLocalControl();
  } else {
    dataDisplay.updateDisplay(controlState);
  }
  delay(100);
}

void handleLocalControl() {
  // Read local control inputs and update control state
  // Example: Read potentiometer value for gain
  int gainValue = analogRead(A0);
  controlState.gainLevel = map(gainValue, 0, 1023, 0, 11);

  // Example: Read button states for other controls
  controlState.phaseState = digitalRead(PHASE_BUTTON_PIN);
  controlState.phantomPowerState = digitalRead(PHANTOM_POWER_BUTTON_PIN);
  controlState.inputImpedanceState = digitalRead(INPUT_IMPEDANCE_BUTTON_PIN);
  controlState.padState = digitalRead(PAD_BUTTON_PIN);
  controlState.highPassFilterState = digitalRead(HIGH_PASS_FILTER_BUTTON_PIN);
  controlState.muteState = digitalRead(MUTE_BUTTON_PIN);

  dataDisplay.updateDisplay(controlState);
  sendControlStatesToCA();
}

void sendControlStatesToCA() {
  Wire.beginTransmission(I2C_BASE_ADDRESS);
  Wire.write(I2C_START_BYTE);
  Wire.write(controlState.gainLevel);
  Wire.write(controlState.phaseState);
  Wire.write(controlState.phantomPowerState);
  Wire.write(controlState.inputImpedanceState);
  Wire.write(controlState.padState);
  Wire.write(controlState.highPassFilterState);
  Wire.write(controlState.muteState);
  Wire.write(I2C_END_BYTE);
  Wire.endTransmission();
}

void receiveControlStates(int numBytes) {
  if (numBytes >= 8) {
    uint8_t startByte = Wire.read();
    if (startByte == I2C_START_BYTE) {
      controlState.gainLevel = Wire.read();
      controlState.phaseState = Wire.read();
      controlState.phantomPowerState = Wire.read();
      controlState.inputImpedanceState = Wire.read();
      controlState.padState = Wire.read();
      controlState.highPassFilterState = Wire.read();
      controlState.muteState = Wire.read();
      uint8_t endByte = Wire.read();
      if (endByte == I2C_END_BYTE) {
        updateDeviceSettings();
        localControl = false;
      }
    }
  }
}

void updateDeviceSettings() {
  // Update device settings based on control states
  // Example: Set digital potentiometer for gain control
  digitalPotWrite(GAIN_CONTROL_PIN, controlState.gainLevel);

  // Example: Set relays for other controls
  digitalWrite(PHASE_RELAY_PIN, controlState.phaseState);
  digitalWrite(PHANTOM_POWER_RELAY_PIN, controlState.phantomPowerState);
  digitalWrite(INPUT_IMPEDANCE_RELAY_PIN, controlState.inputImpedanceState);
  digitalWrite(PAD_RELAY_PIN, controlState.padState);
  digitalWrite(HIGH_PASS_FILTER_RELAY_PIN, controlState.highPassFilterState);
  digitalWrite(MUTE_RELAY_PIN, controlState.muteState);
}

// uint8_t readDIPSwitches() {
//   uint8_t address = I2C_BASE_ADDRESS;
//   for (uint8_t i = 0; i < NUM_DIP_SWITCH_PINS; i++) {
//     pinMode(DIP_SWITCH_PIN_START + i, INPUT_PULLUP);
//     address |= (digitalRead(DIP_SWITCH_PIN_START + i) << i);
//   }
//   return address;
// }