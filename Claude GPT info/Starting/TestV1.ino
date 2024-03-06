// Control arduino Tests
// This is a mock sketch for the CRPi I2C slave device. It simulates the CRPi's I2C communication protocol and control state data.


#include <Wire.h>

// Define mock CRPi I2C address
const uint8_t MOCK_CRPI_ADDRESS = 0x10;

// Define mock control state data
uint8_t mockGainState = 5;
bool mockPhaseState = true;
bool mockPhantomPowerState = false;
// ...

void setup() {
  Wire.begin(MOCK_CRPI_ADDRESS);
  Wire.onRequest(sendMockControlStates);
  Wire.onReceive(receiveMockControlStates);
  Serial.begin(9600);
}

void loop() {
  // Check for incoming serial data to update mock control states
  if (Serial.available()) {
    char command = Serial.read();
    switch (command) {
      case 'g':
        mockGainState = (mockGainState + 1) % 12;
        break;
      case 'p':
        mockPhaseState = !mockPhaseState;
        break;
      // ...
    }
  }
}

void sendMockControlStates() {
  Wire.write(mockGainState);
  Wire.write(mockPhaseState);
  // ...
}

void receiveMockControlStates(int byteCount) {
  while (Wire.available()) {
    uint8_t receivedGainState = Wire.read();
    bool receivedPhaseState = Wire.read();
    // ...
    Serial.print("Received: Gain=");
    Serial.print(receivedGainState);
    Serial.print(", Phase=");
    Serial.println(receivedPhaseState ? "ON" : "OFF");
  }
}


// Testing Plan for CA:
// I2C Address Assignment:
// Test the readDIPSwitches() function by setting different DIP switch combinations and verifying the assigned I2C address.
// Ensure that the I2C address is correctly set during the setup() function.
// Rotary Encoder:
// Test the handleRotaryEncoder() interrupt handler by manually rotating the encoder and verifying the correct gain state changes.
// Test the mute functionality by pressing the rotary encoder switch and verifying the mute state change.
// Verify debouncing implementation for the rotary encoder and switch.
// Momentary Switches:
// Test each momentary switch (phaseButtonPin, phantomPowerButtonPin, inputImpedanceButtonPin, padButtonPin, highPassFilterButtonPin) by pressing it and verifying the corresponding state change.
// Ensure that the interrupt handlers (handlePhase(), etc.) are correctly updating the respective state variables.
// State Change Signaling:
// Implement a mock CRPi and test the checkAndSignalStateChanges() function by simulating state changes and verifying that the multiplexer control pin is correctly triggered.
// I2C Communication:
// Set up a mock CRPi to test the Wire.onReceive() and Wire.onRequest() handlers.
// Test the receiveControlStates() function by sending different control state combinations from the mock CRPi and verifying that the states are correctly updated on the CA.
// Test the sendControlStates() function by requesting control states from the mock CRPi and verifying that the correct state data is sent.
// OLED Display:
// Test the updateDisplay() function by simulating different control state changes and verifying that the OLED display correctly reflects the updated states.

// To use this test code, upload it to an Arduino board connected to the same I2C bus as the CA. Then, you can send serial commands to update the mock control states and verify that the CA correctly receives and processes the state changes. Additionally, you can manually trigger the interrupt handlers for the momentary switches and rotary encoder to test their functionality.