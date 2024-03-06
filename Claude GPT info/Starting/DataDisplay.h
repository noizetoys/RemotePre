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