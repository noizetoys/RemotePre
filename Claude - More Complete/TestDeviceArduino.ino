// TestDeviceArduino.ino
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"
#include "DataDisplay.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);
DataDisplay dataDisplay(display);

ControlState controlState;
uint8_t i2cAddress;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  i2cAddress = 0x08;
}

void loop() {
  testReceiveControlStates();
  testSendControlStatesToCA();
  testUpdateDeviceSettings();
  delay(1000);
}

void testReceiveControlStates() {
  Serial.println("Testing receiveControlStates()...");
  uint8_t testData[] = {0xAA, 5, 1, 1, 0, 0, 1, 1, 0xBB};
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveControlStates);
  Wire.begin();
  Wire.beginTransmission(i2cAddress);
  Wire.write(testData, sizeof(testData));
  Wire.endTransmission();

  delay(100);

  if (controlState.gainLevel == 5 &&
      controlState.phaseState == 1 &&
      controlState.phantomPowerState == 1 &&
      controlState.inputImpedanceState == 0 &&
      controlState.padState == 0 &&
      controlState.highPassFilterState == 1 &&
      controlState.muteState == 1) {
    Serial.println("receiveControlStates() test passed");
  } else {
    Serial.println("receiveControlStates() test failed");
  }
}

void testSendControlStatesToCA() {
  Serial.println("Testing sendControlStatesToCA()...");
  controlState.gainLevel = 7;
  controlState.phaseState = 0;
  controlState.phantomPowerState = 1;
  controlState.inputImpedanceState = 1;
  controlState.padState = 0;
  controlState.highPassFilterState = 1;
  controlState.muteState = 0;

  Wire.begin(i2cAddress);
  Wire.onRequest([]() {
    uint8_t receivedData[9];
    uint8_t index = 0;
    while (Wire.available()) {
      receivedData[index++] = Wire.read();
    }

    if (receivedData[0] == 0xAA &&
        receivedData[1] == 7 &&
        receivedData[2] == 0 &&
        receivedData[3] == 1 &&
        receivedData[4] == 1 &&
        receivedData[5] == 0 &&
        receivedData[6] == 1 &&
        receivedData[7] == 0 &&
        receivedData[8] == 0xBB) {
      Serial.println("sendControlStatesToCA() test passed");
    } else {
      Serial.println("sendControlStatesToCA() test failed");
    }
  });

  sendControlStatesToCA();
}

void testUpdateDeviceSettings() {
  Serial.println("Testing updateDeviceSettings()...");
  controlState.gainLevel = 3;
  controlState.phaseState = 1;
  controlState.phantomPowerState = 0;
  controlState.inputImpedanceState = 1;
  controlState.padState = 0;
  controlState.highPassFilterState = 1;
  controlState.muteState = 1;

  updateDeviceSettings();

  // Verify the device settings
  if (digitalRead(GAIN_CONTROL_PIN) == 3 &&
      digitalRead(PHASE_RELAY_PIN) == 1 &&
      digitalRead(PHANTOM_POWER_RELAY_PIN) == 0 &&
      digitalRead(INPUT_IMPEDANCE_RELAY_PIN) == 1 &&
      digitalRead(PAD_RELAY_PIN) == 0 &&
      digitalRead(HIGH_PASS_FILTER_RELAY_PIN) == 1 &&
      digitalRead(MUTE_RELAY_PIN) == 1) {
    Serial.println("updateDeviceSettings() test passed");
  } else {
    Serial.println("updateDeviceSettings() test failed");
  }
}