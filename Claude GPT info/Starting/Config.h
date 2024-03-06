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

// Button debounce constants
constexpr uint16_t DEBOUNCE_DELAY = 50; // Debounce delay in milliseconds
constexpr uint8_t DEBOUNCE_ITERATIONS = 3; // Number of iterations for debounce