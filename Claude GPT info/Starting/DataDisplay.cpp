// DataDisplay.cpp
#include "DataDisplay.h"

/// @brief Constructor
/// @param display The Adafruit_SSD1306 object for the display
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