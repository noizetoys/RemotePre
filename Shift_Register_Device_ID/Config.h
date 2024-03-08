// Config.h
#pragma once

#include <Arduino.h>


// Screen

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;


// Control Button Pins

constexpr uint8_t BUTTONS_CLOCK_ENABLE_PIN = 2;  // Clock Enable - Pin 15
constexpr uint8_t BUTTONS_DATA_PIN = 3;        // Serial - Pin 9
constexpr uint8_t BUTTONS_CLOCK_PIN = 4;       // Clock Pulse - Pin 2
constexpr uint8_t BUTTONS_LATCH_PIN = 5;       // Latch/Load - Pin 1


// Device ID Pins

constexpr uint8_t DEVICE_ID_CLOCK_ENABLE_PIN = 6;  // Clock Enable - Pin 15
constexpr uint8_t DEVICE_ID_DATA_PIN = 7;         // Serial - Pin 9
constexpr uint8_t DEVICE_ID_CLOCK_PIN = 8;        // Clock Pulse - Pin 2
constexpr uint8_t DEVICE_ID_LATCH_PIN = 9;       // Latch/Load - Pin 1


// Encoder Pins

constexpr uint8_t ROTARY_ENCODER_PIN_A = 12;
constexpr uint8_t ROTARY_ENCODER_PIN_B = 13;


// New Data Trigger Pin

constexpr uint8_t NEW_DATA_TRIGGER_PIN = 12;


// Control Button IDs

constexpr uint8_t PHANTOM_POWER_BUTTON = 1;
constexpr uint8_t PHASE_BUTTON = 2;
constexpr uint8_t INPUT_IMPEDANCE_BUTTON = 4;
constexpr uint8_t PAD_BUTTON = 8;
constexpr uint8_t HIGH_PASS_FILTER_BUTTON = 16;
constexpr uint8_t ENCODER_MUTE_SWITCH = 32;


// Declaration for SSD1306 display connected using software SPI (default case):

constexpr uint8_t OLED_CLK = 9;
constexpr uint8_t OLED_MOSI = 1;
constexpr uint8_t OLED_RESET = 11;
constexpr uint8_t OLED_DC = 12;
constexpr uint8_t OLED_CS = 13;


// Gain values

constexpr int GAIN_VALUES[] = {25, 29, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60};
constexpr size_t NUM_GAIN_VALUES = sizeof(GAIN_VALUES) / sizeof(GAIN_VALUES[0]);

// I2C address calculation
constexpr uint8_t I2C_BASE_ADDRESS = 0x03;

//constexpr uint8_t DIP_SWITCH_PIN_START = 12;
//constexpr uint8_t NUM_DIP_SWITCH_PINS = 8;

// Button debounce constants
constexpr uint16_t DEBOUNCE_DELAY = 50; // Debounce delay in milliseconds
constexpr uint8_t DEBOUNCE_ITERATIONS = 3; // Number of iterations for debounce
