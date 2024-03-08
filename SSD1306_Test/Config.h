#pragma once

#include <Arduino.h>


/// Buttons
constexpr uint8_t PHANTOM_POWER_BUTTON_PIN = 2;
constexpr uint8_t PHASE_BUTTON_PIN = 3;
constexpr uint8_t INPUT_IMPEDANCE_BUTTON_PIN = 4;
constexpr uint8_t PAD_BUTTON_PIN = 5;
constexpr uint8_t HIGH_PASS_FILTER_BUTTON_PIN = 6;

/* Screen */

// Declaration for SSD1306 display connected using software SPI (default case):
constexpr uint8_t  OLED_CLK = 9;
constexpr uint8_t  OLED_MOSI = 10;
constexpr uint8_t  OLED_RESET = 11;
constexpr uint8_t  OLED_DC = 12;
constexpr uint8_t  OLED_CS = 13;

// Screen Dimensions:
constexpr uint8_t  SCREEN_WIDTH 128
constexpr uint8_t  SCREEN_HEIGHT 64

// Encoder
constexpr uint8_t ROTARY_ENCODER_PIN_A = 7;
constexpr uint8_t ROTARY_ENCODER_PIN_B = 8;
// Mute Button
constexpr uint8_t ROTARY_ENCODER_MUTE_SWITCH_PIN = A4;


// Triggers CRPi to request data
constexpr uint8_t MULTI PLEXER_CONTROL_PIN = 11;


// PISO Shift Register for I2C Address (DeviceID) & Future Expansion (8 Pins total)
constexpr uint8_t SHIFT_REGISTER_CLOCK_ENABLE_PIN A0  // Clock Enable - Pin 15
constexpr uint8_t SHIFT_REGISTER_DATA_PIN A1          // Serial - Pin 9
constexpr uint8_t SHIFT_REGISTER_CLOCK_PIN A2         // Clock Pulse - Pin 2
constexpr uint8_t SHIFT_REGISTER_LATCH_PIN A3         // Latch/Load - Pin 1

// Gain values
constexpr int GAIN_VALUES[] = {25, 29, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60};
constexpr size_t NUM_GAIN_VALUES = sizeof(GAIN_VALUES) / sizeof(GAIN_VALUES[0]);

// I2C address calculation
constexpr uint8_t I2C_BASE_ADDRESS = 0x03;

constexpr uint8_t DIP_SWITCH_PIN_START = 12;
constexpr uint8_t NUM_DIP_SWITCH_PINS = 8;

// Button debounce constants
constexpr uint16_t DEBOUNCE_DELAY = 50; // Debounce delay in milliseconds
constexpr uint8_t DEBOUNCE_ITERATIONS = 3; // Number of iterations for debounce