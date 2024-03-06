#include <Wire.h>

// Pin definitions for digital potentiometer
#define POT_CS_PIN 10
#define RELAY_PINS_START 2
#define NUM_RELAYS 6

// Pin definitions for PISO shift register
#define SHIFT_REGISTER_DATA_PIN A0    // Connect to SER (serial data) pin of shift register
#define SHIFT_REGISTER_CLOCK_PIN A1   // Connect to SRCLK (shift register clock) pin of shift register
#define SHIFT_REGISTER_LATCH_PIN A2   // Connect to RCLK (register clock) pin of shift register
#define NUM_DIP_SWITCHES 5

struct DeviceData {
    uint8_t device_id;
    bool bool_states[NUM_RELAYS];
    uint16_t volume;
    bool mute;
};

void setup() {
    Serial.begin(9600);
    // Read DIP switches to determine device ID
    uint8_t device_id = readDipSwitches();

    // Set SLAVE_ADDRESS based on device ID
    int SLAVE_ADDRESS = device_id;

    Wire.begin(SLAVE_ADDRESS);       // join i2c bus with address
    Wire.onReceive(receiveEvent);    // register receive event

    pinMode(POT_CS_PIN, OUTPUT);
    for (int i = 0; i < NUM_RELAYS; ++i) {
        pinMode(RELAY_PINS_START + i, OUTPUT);
    }
    pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);
    pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);

    Serial.print("Device ID: ");
    Serial.println(device_id);
    Serial.print("SLAVE_ADDRESS: ");
    Serial.println(SLAVE_ADDRESS);
}

void loop() {
    // Your Arduino code
}

void receiveEvent(int length) {
    if (length == sizeof(DeviceData) + 1) { // Add 1 for CRC byte
        DeviceData data;
        uint8_t data_with_crc[length];
        Wire.readBytes(data_with_crc, length);

        // Validate CRC
        if (validateCRC(data_with_crc, length)) {
            // CRC is valid, process data
            memcpy(&data, data_with_crc, sizeof(DeviceData));

            // Set digital potentiometer
            setDigitalPot(data.volume);

            // Set relays based on bool states
            for (int i = 0; i < NUM_RELAYS; ++i) {
                digitalWrite(RELAY_PINS_START + i, data.bool_states[i]);
            }

            // Print received data
            Serial.println("Received data:");
            Serial.print("Device ID: ");
            Serial.println(data.device_id);
            Serial.print("Volume: ");
            Serial.println(data.volume);
            Serial.print("Mute: ");
            Serial.println(data.mute);
            Serial.println("Bool States:");
            for (int i = 0; i < NUM_RELAYS; ++i) {
                Serial.print("Relay ");
                Serial.print(i);
                Serial.print(": ");
                Serial.println(data.bool_states[i]);
            }
        } else {
            // CRC validation failed
            Serial.println("CRC validation failed");
            // Handle CRC validation failure
        }
    } else {
        // Discard invalid data
        while (Wire.available()) {
            Wire.read();
        }
        Serial.println("Invalid data received");
    }
}

void setDigitalPot(uint16_t value) {
    // Function to set digital potentiometer
    digitalWrite(POT_CS_PIN, LOW);
    // Send SPI command to set digital potentiometer value
    // Example:
    // SPI.transfer(0b00010001);  // Command to set potentiometer 0 (for example)
    // SPI.transfer(highByte(value));
    // SPI.transfer(lowByte(value));
    digitalWrite(POT_CS_PIN, HIGH);
}

bool validateCRC(uint8_t* data_with_crc, int length) {
    // Validate CRC checksum for received data
    uint8_t crc = data_with_crc[length - 1];
    uint8_t calculated_crc = calculateCRC(data_with_crc, length - 1); // Exclude CRC byte
    return crc == calculated_crc;
}

uint8_t calculateCRC(uint8_t* data, size_t length) {
    // Calculate CRC checksum for data
    const uint8_t polynomial = 0x31; // CRC-8-CCITT polynomial
    uint8_t crc = 0xFF; // Initial value

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; ++bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint8_t readDipSwitches() {
    // Read DIP switches from PISO shift register
    digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
    delayMicroseconds(5);  // Allow time for the latch to settle
    digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
    int id = 0;
    for (int i = 0; i < NUM_DIP_SWITCHES; ++i) {
        id |= digitalRead(SHIFT_REGISTER_DATA_PIN) << i;
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);
        delayMicroseconds(5);  // Allow time for the data to be read
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, LOW);
    }
    return id;
}
