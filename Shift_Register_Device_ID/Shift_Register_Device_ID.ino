
// Device ID
// #define SHIFT_REGISTER_DATA_PIN 4          // Serial - Pin 9
// #define SHIFT_REGISTER_CLOCK_PIN 6         // Clock Pulse - Pin 2
// #define SHIFT_REGISTER_LATCH_PIN 7         // Latch/Load - Pin 1
// #define SHIFT_REGISTER_CLOCK_ENABLE_PIN 3  // Clock Enable - Pin 15

// Device ID
#define SHIFT_REGISTER_CLOCK_ENABLE_PIN 10  // Clock Enable - Pin 15
#define SHIFT_REGISTER_DATA_PIN 11          // Serial - Pin 9
#define SHIFT_REGISTER_CLOCK_PIN 12         // Clock Pulse - Pin 2
#define SHIFT_REGISTER_LATCH_PIN 13         // Latch/Load - Pin 1


void setup() {
  Serial.begin(115200);
  Serial.println("\n--> Setup");

  configure_DeviceID_Pins();

  // Needs to be done twice to remove stale data
  int deviceID = loadIDFromShiftRegister();
  deviceID = loadIDFromShiftRegister();

  Serial.print("Device ID:  ");
  Serial.println(deviceID);
}

void loop() {
}


void configure_DeviceID_Pins() {
  pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);

  pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);

  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);
  delayMicroseconds(5);
}


int loadIDFromShiftRegister() {
  // Pulse Pins
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
  delayMicroseconds(5);

  digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, LOW);

  // Use LSBFirst if displaying the bits for debugging ONLY
  byte incoming = shiftIn(SHIFT_REGISTER_CLOCK_ENABLE_PIN, SHIFT_REGISTER_CLOCK_PIN, LSBFIRST);

  // byte incoming = shiftIn(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, MSBFIRST);
  Serial.println("loadIDFromShiftRegister");
    Serial.println(incoming, BIN);

  digitalWrite(SHIFT_REGISTER_CLOCK_ENABLE_PIN, HIGH);

  return incoming;
}

