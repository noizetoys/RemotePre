// Control Arduino
// This code is for the Arduino that is connected to the control panel. It reads the state of the rotary encoder and buttons, and sends the state of the controls to the CRPi.


#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define pin assignments
const int rotaryEncoderPinA = 2;
const int rotaryEncoderPinB = 3;
const int rotaryEncoderSwitch = 4;
const int phaseButtonPin = 5;
const int phantomPowerButtonPin = 6;
const int inputImpedanceButtonPin = 7;
const int padButtonPin = 8;
const int highPassFilterButtonPin = 9;
const int multiplexerControlPin = 10;

// Define I2C address and control states
uint8_t i2cAddress;
int gainState = 0;
bool phaseState = false;
bool phantomPowerState = false;
bool inputImpedanceState = false;
bool padState = false;
bool highPassFilterState = false;

// Rotary encoder object
Rotary rotary = Rotary(rotaryEncoderPinA, rotaryEncoderPinB);

// OLED display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// DataDisplay object
DataDisplay dataDisplay(display, {
    gainState,
    phaseState,
    phantomPowerState,
    inputImpedanceState,
    padState,
    highPassFilterState
});

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up I2C address using DIP switches
  i2cAddress = readDIPSwitches();
  Wire.begin(i2cAddress);

  // Configure pin modes
  pinMode(rotaryEncoderSwitch, INPUT_PULLUP);
  pinMode(phaseButtonPin, INPUT_PULLUP);
  pinMode(phantomPowerButtonPin, INPUT_PULLUP);
  pinMode(inputImpedanceButtonPin, INPUT_PULLUP);
  pinMode(padButtonPin, INPUT_PULLUP);
  pinMode(highPassFilterButtonPin, INPUT_PULLUP);

  // Attach interrupt handlers
  attachInterrupt(digitalPinToInterrupt(rotaryEncoderPinA), handleRotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryEncoderPinB), handleRotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryEncoderSwitch), handleMute, FALLING);
  attachInterrupt(digitalPinToInterrupt(phaseButtonPin), handlePhase, RISING);
  attachInterrupt(digitalPinToInterrupt(phantomPowerButtonPin), handlePhantomPower, RISING);
  attachInterrupt(digitalPinToInterrupt(inputImpedanceButtonPin), handleInputImpedance, RISING);
  attachInterrupt(digitalPinToInterrupt(padButtonPin), handlePad, RISING);
  attachInterrupt(digitalPinToInterrupt(highPassFilterButtonPin), handleHighPassFilter, RISING);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Initialize control states
  Wire.onReceive(receiveControlStates);

  // Update OLED display
  dataDisplay.updateDisplay();
}

void loop() {
  // Check for state changes and signal CRPi if necessary
  checkAndSignalStateChanges();

  // Handle incoming I2C requests
  Wire.onRequest(sendControlStates);
}

// Rotary encoder interrupt handler
void handleRotaryEncoder() {
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    gainState = (gainState + 1) % 12;
    dataDisplay.updateDisplay();
  } else if (result == DIR_CCW) {
    gainState = (gainState - 1 + 12) % 12;
    dataDisplay.updateDisplay();
  }
}

// Mute interrupt handler
void handleMute() {
  // Handle mute state change
  dataDisplay.updateDisplay();
}

// Phase button interrupt handler
void handlePhase() {
  phaseState = !phaseState;
  dataDisplay.updateDisplay();
}

// Phantom power button interrupt handler
void handlePhantomPower() {
  phantomPowerState = !phantomPowerState;
  dataDisplay.updateDisplay();
}

// Input impedance button interrupt handler
void handleInputImpedance() {
  inputImpedanceState = !inputImpedanceState;
  dataDisplay.updateDisplay();
}

// Pad button interrupt handler
void handlePad() {
  padState = !padState;
  dataDisplay.updateDisplay();
}

// High pass filter button interrupt handler
void handleHighPassFilter() {
  highPassFilterState = !highPassFilterState;
  dataDisplay.updateDisplay();
}

// Read DIP switches to set I2C address
uint8_t readDIPSwitches() {
  uint8_t address = 0x03;
  for (int i = 0; i < 5; i++) {
    int pinValue = digitalRead(i + 11);
    address |= (pinValue << i);
  }
  return address;
}

// Check for state changes and signal CRPi
void checkAndSignalStateChanges() {
  static int prevGainState = gainState;
  static bool prevPhaseState = phaseState;
  static bool prevPhantomPowerState = phantomPowerState;
  static bool prevInputImpedanceState = inputImpedanceState;
  static bool prevPadState = padState;
  static bool prevHighPassFilterState = highPassFilterState;

  bool stateChanged = false;

  if (gainState != prevGainState) {
    prevGainState = gainState;
    stateChanged = true;
  }

  if (phaseState != prevPhaseState) {
    prevPhaseState = phaseState;
    stateChanged = true;
  }

  if (phantomPowerState != prevPhantomPowerState) {
    prevPhantomPowerState = phantomPowerState;
    stateChanged = true;
  }

  if (inputImpedanceState != prevInputImpedanceState) {
    prevInputImpedanceState = inputImpedanceState;
    stateChanged = true;
  }

  if (padState != prevPadState) {
    prevPadState = padState;
    stateChanged = true;
  }

  if (highPassFilterState != prevHighPassFilterState) {
    prevHighPassFilterState = highPassFilterState;
    stateChanged = true;
  }

  if (stateChanged) {
    // Signal CRPi using multiplexer control pin
    digitalWrite(multiplexerControlPin, HIGH);
    delay(10);
    digitalWrite(multiplexerControlPin, LOW);
  }
}

// Send control states over I2C
void sendControlStates() {
  Wire.write(gainState);
  Wire.write(phaseState);
  Wire.write(phantomPowerState);
  Wire.write(inputImpedanceState);
  Wire.write(padState);
  Wire.write(highPassFilterState);
}

// Receive control states over I2C
void receiveControlStates(int byteCount) {
  if (Wire.available() >= 6) {
    gainState = Wire.read();
    phaseState = Wire.read();
    phantomPowerState = Wire.read();
    inputImpedanceState = Wire.read();
    padState = Wire.read();
    highPassFilterState = Wire.read();
    dataDisplay.updateDisplay();
  }
}