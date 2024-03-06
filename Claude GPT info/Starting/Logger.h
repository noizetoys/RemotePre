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