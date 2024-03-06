#include "HardwareSerial.h"


#include "DeviceIDReader.h"


int DeviceIDReader::SHIFT_REGISTER_DATA_PIN = 4;
int DeviceIDReader::SHIFT_REGISTER_LATCH_PIN = 7;
int DeviceIDReader::SHIFT_REGISTER_CLOCK_PIN = 6;
int DeviceIDReader::SHIFT_REGISTER_CLOCK_ENABLE_PIN = 3;


int DeviceIDReader::retrieveDeviceID() {
  Serial.println("\nDeviceIDReader::retrieveDeviceID()");

  // pinMode(DeviceIDReader::SHIFT_REGISTER_DATA_PIN, INPUT);

  // pinMode(DeviceIDReader::SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  // pinMode(DeviceIDReader::SHIFT_REGISTER_CLOCK_ENABLE_PIN, OUTPUT);
  // pinMode(DeviceIDReader::SHIFT_REGISTER_LATCH_PIN, OUTPUT);

  // // Enable Clock
  // digitalWrite(DeviceIDReader::SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);
  pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);

  pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);

  // Enable Clock
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);


  Serial.print("\nDeviceIDReader::SHIFT_REGISTER_DATA_PIN = ");
  Serial.println(SHIFT_REGISTER_DATA_PIN);
  Serial.print("DeviceIDReader::SHIFT_REGISTER_LATCH_PIN = ");
  Serial.println(SHIFT_REGISTER_LATCH_PIN);
  Serial.print("DeviceIDReader::SHIFT_REGISTER_CLOCK_PIN = ");
  Serial.println(SHIFT_REGISTER_CLOCK_PIN);
  Serial.print("DeviceIDReader::SHIFT_REGISTER_CLOCK_ENABLE_PIN = ");
  Serial.println(SHIFT_REGISTER_CLOCK_ENABLE_PIN);

  // DeviceIDReader::configure();

  DeviceIDReader::pulseShiftRegister();

  // Done twice to clear stale data
  byte dipSwitchSettings = DeviceIDReader::readValueFromShiftRegister();
  delay(20);
  dipSwitchSettings = DeviceIDReader::readValueFromShiftRegister();
  Serial.print("\nDevice ID:  ");
  Serial.println(dipSwitchSettings, BIN);

  // delay(1000);

  return int(dipSwitchSettings);
}


void DeviceIDReader::configure() {
  Serial.println("\nDeviceIDReader::configure()");
  // Serial.begin(115200);

  // Serial Input
  pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);

  pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);

  // Enable Clock
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);
}


byte DeviceIDReader::readValueFromShiftRegister() {
  Serial.println("\nDeviceIDReader::readValueFromShiftRegister()");

  // Get data from 74HC165
  digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);

  // Enable clock (LOW = Trigger)
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);

  // Read Serial
  // Use LSBFirst if displaying the bits for debugging ONLY
  byte incoming = shiftIn(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, LSBFIRST);
  // byte incoming = shiftIn(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, MSBFIRST);
  Serial.print("incoming byte = ");
  Serial.print(incoming, BIN);
  Serial.print(", as an int = ");
  Serial.println(incoming, DEC);

  // Disable Clock
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, HIGH);

  return int(incoming);
}


void DeviceIDReader::pulseShiftRegister() {
  Serial.println("\nDeviceIDReader::pulseShiftRegister()");

  digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
  delayMicroseconds(5);
}
