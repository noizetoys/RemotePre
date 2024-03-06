// Logger.h
#pragma once

#include <Arduino.h>

class Logger {
public:
  static void init(uint32_t baudRate = 115200) {
    Serial.begin(baudRate);
  }

  template<typename... Args>
  static void debug(const char* format, Args... args) {
    Serial.print("[DEBUG] ");
    Serial.printf(format, args...);
    Serial.println();
  }

  template<typename... Args>
  static void info(const char* format, Args... args) {
    Serial.print("[INFO] ");
    Serial.printf(format, args...);
    Serial.println();
  }

  template<typename... Args>
  static void error(const char* format, Args... args) {
    Serial.print("[ERROR] ");
    Serial.printf(format, args...);
    Serial.println();
  }
};