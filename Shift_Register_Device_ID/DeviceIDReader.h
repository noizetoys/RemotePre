
#ifndef DEVICEIDREADER_H
#define DEVICEIDREADER_H

#include <Arduino.h>

class DeviceIDReader {
public:
  // Serial Output - Pin 9
  static int SHIFT_REGISTER_DATA_PIN;

  // Latch/Load - Pin 1
  static int SHIFT_REGISTER_LATCH_PIN;

  // Clock Pulse - Pin 2
  static int SHIFT_REGISTER_CLOCK_PIN;

  // Clock Enable -  Pin 15
  static int SHIFT_REGISTER_CLOCK_ENABLE_PIN;

  static void configure();
  static uint8_t readValueFromShiftRegister();
  static void pulseShiftRegister();

// public:
  static int retrieveDeviceID();
};

#endif