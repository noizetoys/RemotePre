// // Config.h
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

// Button debounce constants
constexpr uint16_t DEBOUNCE_DELAY = 50; // Debounce delay in milliseconds
// constexpr uint8_t DEBOUNCE_ITERATIONS = 3; // Number of iterations for debounce

// Logger.h
#pragma once

#include <Arduino.h>

/// @brief Logger class for printing debug, info, and error messages
class Logger {
public:
    /// @brief Initialize the logger with the specified baud rate
    /// @param baudRate The baud rate for serial communication
    static void init(uint32_t baudRate = 115200) {
        Serial.begin(baudRate);
    }

    /// @brief Print a debug message
    /// @param message The message to be printed
    static void debug(const char* message) {
        Serial.print("[DEBUG] ");
        Serial.println(message);
    }

    /// @brief Print an info message
    /// @param message The message to be printed
    static void info(const char* message) {
        Serial.print("[INFO] ");
        Serial.println(message);
    }

    /// @brief Print an error message
    /// @param message The message to be printed
    static void error(const char* message) {
        Serial.print("[ERROR] ");
        Serial.println(message);
    }
};

// ControlState.h
#pragma once

/// @brief Structure representing the control state
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

/// @brief Class for managing control states
class StateManager {
public:
    /// @brief Constructor
    StateManager();

    /// @brief Update the gain state
    /// @param newGainState The new gain state
    void updateGainState(int newGainState);

    /// @brief Toggle the phase state
    void togglePhaseState();

    /// @brief Toggle the phantom power state
    void togglePhantomPowerState();

    /// @brief Toggle the input impedance state
    void toggleInputImpedanceState();

    /// @brief Toggle the pad state
    void togglePadState();

    /// @brief Toggle the high pass filter state
    void toggleHighPassFilterState();

    /// @brief Get the current control state
    /// @return The current control state
    ControlState getControlState() const;

    /// @brief Check if the state has changed
    /// @return True if the state has changed, false otherwise
    bool isStateChanged() const;

    /// @brief Reset the state changed flag
    void resetStateChanged();

private:
    ControlState _controlState;
    bool _stateChanged;
};

// // StateManager.cpp
#include "StateManager.h"

/// @brief Constructor
StateManager::StateManager()
    : _controlState{0, false, false, false, false, false}, _stateChanged(false) {}

/// @brief Update the gain state
/// @param newGainState The new gain state
void StateManager::updateGainState(int newGainState) {
    if (_controlState.gainLevel != newGainState) {
        _controlState.gainLevel = newGainState;
        _stateChanged = true;
    }
}

/// @brief Toggle the phase state
void StateManager::togglePhaseState() {
    _controlState.phaseState = !_controlState.phaseState;
    _stateChanged = true;
}

/// @brief Toggle the phantom power state
void StateManager::togglePhantomPowerState() {
    _controlState.phantomPowerState = !_controlState.phantomPowerState;
    _stateChanged = true;
}

/// @brief Toggle the input impedance state
void StateManager::toggleInputImpedanceState() {
    _controlState.inputImpedanceState = !_controlState.inputImpedanceState;
    _stateChanged = true;
}

/// @brief Toggle the pad state
void StateManager::togglePadState() {
    _controlState.padState = !_controlState.padState;
    _stateChanged = true;
}

/// @brief Toggle the high pass filter state
void StateManager::toggleHighPassFilterState() {
    _controlState.highPassFilterState = !_controlState.highPassFilterState;
    _stateChanged = true;
}

/// @brief Get the current control state
/// @return The current control state
ControlState StateManager::getControlState() const {
    return _controlState;
}

/// @brief Check if the state has changed
/// @return True if the state has changed, false otherwise
bool StateManager::isStateChanged() const {
    return _stateChanged;
}

/// @brief Reset the state changed flag
void StateManager::resetStateChanged() {
    _stateChanged = false;
}

// DataDisplay.h
#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ControlState.h"

/// @brief Class for displaying control states on the OLED display
class DataDisplay {
public:
    /// @brief Constructor
    /// @param display The Adafruit_SSD1306 object for the display
    DataDisplay(Adafruit_SSD1306& display);

    /// @brief Update the display with the current control state
    /// @param state The current control state
    void updateDisplay(const ControlState& state);

    /// @brief Display the device ID on the OLED display
    /// @param id The device ID
    void displayDeviceID(int id);

private:
    Adafruit_SSD1306& _display;

    /// @brief Reset the text style
    void resetText();

    /// @brief Display the phantom power state
    /// @param state The phantom power state
    void displayPhantom(bool state);

    /// @brief Display the phase state
    /// @param state The phase state
    void displayPolarity(bool state);

    /// @brief Display the input impedance state
    /// @param state The input impedance state
    void displayInputZ(bool state);

    /// @brief Display the pad state
    /// @param state The pad state
    void displayPad(bool state);

    /// @brief Display the high pass filter state
    /// @param state The high pass filter state
    void displayHPF(bool state);

    /// @brief Display the gain level
    /// @param level The gain level
    void displayGain(int level);

    /// @brief Display all control states
    /// @param state The current control state
    void displayAllItems(const ControlState& state);

    /// @brief Display the mute message
    void displayMute();
};

// DataDisplay.cpp
#include "DataDisplay.h"

// @brief Constructor
// @param display The Adafruit_SSD1306 object for the display
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

/// @brief Update the display with the current control state
/// @param state The current control state
void DataDisplay::updateDisplay(const ControlState& state) {
    if (!state.phaseState) {
        displayMute();
    } else {
        displayAllItems(state);
    }
}

/// @brief Display the device ID on the OLED display
/// @param id The device ID
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

/// @brief Reset the text style
void DataDisplay::resetText() {
    _display.setTextColor(WHITE);
    _display.setTextSize(2);
    _display.setFont(nullptr);
}

/// @brief Display the phantom power state
/// @param state The phantom power state
void DataDisplay::displayPhantom(bool state) {
    resetText();
    _display.setCursor(1, 1);
    if (state) {
        _display.println("48v");
    }
}

/// @brief Display the phase state
/// @param state The phase state
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

/// @brief Display the input impedance state
/// @param state The input impedance state
void DataDisplay::displayInputZ(bool state) {
    resetText();
    _display.setCursor(92, 1);
    _display.println(state ? "HiZ" : "LoZ");
}

/// @brief Display the pad state
/// @param state The pad state
void DataDisplay::displayPad(bool state) {
    resetText();
    if (state) {
        _display.setCursor(92, 26);
        _display.println("-20");
    }
}

/// @brief Display the high pass filter state
/// @param state The high pass filter state
void DataDisplay::displayHPF(bool state) {
    resetText();
    if (state) {
        _display.drawLine(92, 64, 100, 52, WHITE);
        _display.drawFastHLine(100, 52, 27, WHITE);
    }
}

/// @brief Display the gain level
/// @param level The gain level
void DataDisplay::displayGain(int level) {
    _display.setTextSize(3);
    _display.setCursor(1, 35);
    _display.println("+");
    _display.setTextSize(5);
    _display.setCursor(20, 28);
    _display.println(GAIN_VALUES[level]);
}

/// @brief Display all control states
/// @param state The current control state
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

/// @brief Display the mute message
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

// Button state machine
enum ButtonState {
    BUTTON_RELEASED,
    BUTTON_PRESSED,
    BUTTON_DEBOUNCED
};

ButtonState phaseButtonState = BUTTON_RELEASED;
ButtonState phantomPowerButtonState = BUTTON_RELEASED;
ButtonState inputImpedanceButtonState = BUTTON_RELEASED;
ButtonState padButtonState = BUTTON_RELEASED;
ButtonState highPassFilterButtonState = BUTTON_RELEASED;

uint8_t debounceIterations = 0;

// I2C protocol
const uint8_t I2C_START_BYTE = 0xAA;
const uint8_t I2C_END_BYTE = 0xBB;
const uint8_t I2C_BUFFER_SIZE = 32;
uint8_t i2cTxBuffer[I2C_BUFFER_SIZE];
uint8_t i2cRxBuffer[I2C_BUFFER_SIZE];
uint8_t i2cTxBufferIndex = 0;
uint8_t i2cRxBufferIndex = 0;

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
    attachInterrupt(digitalPinToInterrupt(PHASE_BUTTON_PIN), handlePh