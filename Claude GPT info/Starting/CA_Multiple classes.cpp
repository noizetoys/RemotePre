// Config.h
#pragma once

// Pin assignments
constexpr uint8_t ROTARY_ENCODER_PIN_A = 2;
constexpr uint8_t ROTARY_ENCODER_PIN_B = 3;
constexpr uint8_t ROTARY_ENCODER_SWITCH_PIN = 4;
constexpr uint8_t PHASE_BUTTON_PIN = 5;
constexpr uint8_t PHANTOM_POWER_BUTTON_PIN = 6;
constexpr uint8_t INPUT_IMPEDANCE_BUTTON_PIN = 7;
constexpr uint8_t PAD_BUTTON_PIN = 8;
constexpr uint8_t HIGH_PASS_FILTER_BUTTON_PIN = 9;
constexpr uint8_t MULTIPLEXER_CONTROL_PIN = 10;

// Gain values
constexpr int GAIN_VALUES[] = {25, 29, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60};
constexpr size_t NUM_GAIN_VALUES = sizeof(GAIN_VALUES) / sizeof(GAIN_VALUES[0]);

// I2C address calculation
constexpr uint8_t I2C_BASE_ADDRESS = 0x03;
constexpr uint8_t DIP_SWITCH_PIN_START = 11;
constexpr uint8_t NUM_DIP_SWITCH_PINS = 5;

// Logger.h
#pragma once

#include <Arduino.h>

class Logger {
public:
    static void init(uint32_t baudRate = 115200) {
        Serial.begin(baudRate);
    }

    static void debug(const char* message) {
        Serial.print("[DEBUG] ");
        Serial.println(message);
    }

    static void info(const char* message) {
        Serial.print("[INFO] ");
        Serial.println(message);
    }

    static void error(const char* message) {
        Serial.print("[ERROR] ");
        Serial.println(message);
    }
};

// ControlState.h
#pragma once

struct ControlState {
    int gainLevel;
    bool phaseState;
    bool phantomPowerState;
    bool inputImpedanceState;
    bool padState;
    bool highPassFilterState;
};

// StateManager.h
#pragma once

#include "ControlState.h"

class StateManager {
public:
    StateManager();

    void updateGainState(int newGainState);
    void togglePhaseState();
    void togglePhantomPowerState();
    void toggleInputImpedanceState();
    void togglePadState();
    void toggleHighPassFilterState();

    ControlState getControlState() const;
    bool isStateChanged() const;
    void resetStateChanged();

private:
    ControlState _controlState;
    bool _stateChanged;
};

// StateManager.cpp
#include "StateManager.h"

StateManager::StateManager()
    : _controlState{0, false, false, false, false, false}, _stateChanged(false) {}

void StateManager::updateGainState(int newGainState) {
    if (_controlState.gainLevel != newGainState) {
        _controlState.gainLevel = newGainState;
        _stateChanged = true;
    }
}

void StateManager::togglePhaseState() {
    _controlState.phaseState = !_controlState.phaseState;
    _stateChanged = true;
}

void StateManager::togglePhantomPowerState() {
    _controlState.phantomPowerState = !_controlState.phantomPowerState;
    _stateChanged = true;
}

void StateManager::toggleInputImpedanceState() {
    _controlState.inputImpedanceState = !_controlState.inputImpedanceState;
    _stateChanged = true;
}

void StateManager::togglePadState() {
    _controlState.padState = !_controlState.padState;
    _stateChanged = true;
}

void StateManager::toggleHighPassFilterState() {
    _controlState.highPassFilterState = !_controlState.highPassFilterState;
    _stateChanged = true;
}

ControlState StateManager::getControlState() const {
    return _controlState;
}

bool StateManager::isStateChanged() const {
    return _stateChanged;
}

void StateManager::resetStateChanged() {
    _stateChanged = false;
}

// DataDisplay.h
#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"

class DataDisplay {
public:
    DataDisplay(Adafruit_SSD1306& display);
    void updateDisplay(const ControlState& state);
    void displayDeviceID(int id);

private:
    Adafruit_SSD1306& _display;

    void resetText();
    void displayPhantom(bool state);
    void displayPolarity(bool state);
    void displayInputZ(bool state);
    void displayPad(bool state);
    void displayHPF(bool state);
    void displayGain(int level);
    void displayAllItems(const ControlState& state);
    void displayMute();
};

// DataDisplay.cpp
#include "DataDisplay.h"

DataDisplay::DataDisplay(Adafruit_SSD1306& display)
    : _display(display) {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Logger::error("SSD1306 allocation failed");
        while (true) {
            delay(100);
        }
    }

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(WHITE);
}

void DataDisplay::updateDisplay(const ControlState& state) {
    if (!state.phaseState) {
        displayMute();
    } else {
        displayAllItems(state);
    }
}

void DataDisplay::displayDeviceID(int id) {
    _display.clearDisplay();
    _display.invertDisplay(false);
    _display.setTextColor(WHITE);
    _display.setTextSize(5);
    _display.setFont(nullptr);

    if (id < 10) {
        _display.setCursor(36, 23);
    } else {
        _display.setCursor(20, 23);
    }

    _display.print("#");
    _display.println(id);
    _display.display();
    delay(2000);
}

void DataDisplay::resetText() {
    _display.setTextColor(WHITE);
    _display.setTextSize(2);
    _display.setFont(nullptr);
}

void DataDisplay::displayPhantom(bool state) {
    resetText();
    _display.setCursor(1, 1);
    if (state) {
        _display.println("48v");
    }
}

void DataDisplay::displayPolarity(bool state) {
    resetText();
    if (state) {
        _display.drawCircle(63, 8, 6, WHITE);
        _display.drawCircle(63, 8, 7, WHITE);
        _display.drawLine(54, 15, 70, 0, WHITE);
        _display.drawLine(55, 15, 71, 0, WHITE);
        _display.drawLine(56, 15, 72, 0, WHITE);
    }
}

void DataDisplay::displayInputZ(bool state) {
    resetText();
    _display.setCursor(92, 1);
    _display.println(state ? "HiZ" : "LoZ");
}

void DataDisplay::displayPad(bool state) {
    resetText();
    if (state) {
        _display.setCursor(92, 26);
        _display.println("-20");
    }
}

void DataDisplay::displayHPF(bool state) {
    resetText();
    if (state) {
        _display.drawLine(92, 64, 100, 52, WHITE);
        _display.drawFastHLine(100, 52, 27, WHITE);
    }
}

void DataDisplay::displayGain(int level) {
    _display.setTextSize(3);
    _display.setCursor(1, 35);
    _display.println("+");
    _display.setTextSize(5);
    _display.setCursor(20, 28);
    _display.println(GAIN_VALUES[level]);
}

void DataDisplay::displayAllItems(const ControlState& state) {
    _display.clearDisplay();
    _display.invertDisplay(false);
    _display.setTextColor(WHITE);
    _display.setTextSize(2);
    _display.setFont(nullptr);

    displayPhantom(state.phantomPowerState);
    displayPolarity(state.phaseState);
    displayInputZ(state.inputImpedanceState);
    displayPad(state.padState);
    displayHPF(state.highPassFilterState);
    displayGain(state.gainLevel);

    _display.display();
}

void DataDisplay::displayMute() {
    _display.clearDisplay();
    _display.invertDisplay(true);
    _display.drawRect(0, 0, 128, 16, WHITE);
    _display.setTextColor(WHITE);
    _display.setTextSize(5);
    _display.setFont(nullptr);
    _display.setCursor(7, 23);
    _display.println("MUTE");
    _display.display();
    delay(2000);
}

// ControlArduino.ino
#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"
#include "Logger.h"
#include "ControlState.h"
#include "StateManager.h"
#include "DataDisplay.h"

Rotary rotary(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B);
StateManager stateManager;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DataDisplay dataDisplay(display);

uint8_t i2cAddress;

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

    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_A), handleRotaryEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_B), handleRotaryEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_SWITCH_PIN), handleMute, FALLING);
    attachInterrupt(digitalPinToInterrupt(PHASE_BUTTON_PIN), handlePhase, RISING);
    attachInterrupt(digitalPinToInterrupt(PHANTOM_POWER_BUTTON_PIN), handlePhantomPower, RISING);
    attachInterrupt(digitalPinToInterrupt(INPUT_IMPEDANCE_BUTTON_PIN), handleInputImpedance, RISING);
    attachInterrupt(digitalPinToInterrupt(PAD_BUTTON_PIN), handlePad, RISING);
    attachInterrupt(digitalPinToInterrupt(HIGH_PASS_FILTER_BUTTON_PIN), handleHighPassFilter, RISING);

    Wire.onReceive(receiveControlStates);

    dataDisplay.updateDisplay(stateManager.getControlState());

    Logger::info("Control Arduino setup completed");
}

void loop() {
    checkAndSignalStateChanges();
    Wire.onRequest(sendControlStates);
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

void handleMute() {
    stateManager.togglePhaseState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handlePhase() {
    stateManager.togglePhaseState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handlePhantomPower() {
    stateManager.togglePhantomPowerState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handleInputImpedance() {
    stateManager.toggleInputImpedanceState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handlePad() {
    stateManager.togglePadState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handleHighPassFilter() {
    stateManager.toggleHighPassFilterState();
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

void sendControlStates() {
    ControlState state = stateManager.getControlState();
    Wire.write(state.gainLevel);
    Wire.write(state.phaseState);
    Wire.write(state.phantomPowerState);
    Wire.write(state.inputImpedanceState);
    Wire.write(state.padState);
    Wire.write(state.highPassFilterState);
}

void receiveControlStates(int byteCount) {
    if (Wire.available() >= 6) {
        ControlState newState;
        newState.gainLevel = Wire.read();
        newState.phaseState = Wire.read();
        newState.phantomPowerState = Wire.read();
        newState.inputImpedanceState = Wire.read();
        newState.padState = Wire.read();
        newState.highPassFilterState = Wire.read();

        if (newState != stateManager.getControlState()) {
            stateManager.updateGainState(newState.gainLevel);
            stateManager.togglePhaseState();
            stateManager.togglePhantomPowerState();
            stateManager.toggleInputImpedanceState();
            stateManager.togglePadState();
            stateManager.toggleHighPassFilterState();
            dataDisplay.updateDisplay(stateManager.getControlState());
        }
    }
}