
// Control_Arduion_4


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions for digital potentiometer
#define POT_CS_PIN 10
#define RELAY_PINS_START 2
#define NUM_RELAYS 6

// Pin definitions for PISO shift register
#define SHIFT_REGISTER_DATA_PIN A0    // Connect to SER (serial data) pin of shift register
#define SHIFT_REGISTER_CLOCK_PIN A1   // Connect to SRCLK (shift register clock) pin of shift register
#define SHIFT_REGISTER_LATCH_PIN A2   // Connect to RCLK (register clock) pin of shift register
#define NUM_DIP_SWITCHES 5

// Pin definitions for buttons
#define BUTTON_PIN 3
#define NUM_BUTTONS 1

// Rotary encoder settings
#define ROTARY_PIN_A 4
#define ROTARY_PIN_B 5
#define ROTARY_BUTTON_PIN 6
#define ROTARY_ENCODER_STEPS 12

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Control data struct
struct ControlData {
    uint8_t device_id;
    bool bool_states[NUM_RELAYS];
    uint16_t volume;
    bool mute;
};

// Switch states
bool pad_state = false;
bool polarity_state = false;
bool hpf_state = false;
bool phantom_power_state = false;
bool input_impedance_state = false;

// Button states
bool button_state = false;
bool last_button_state = false;

// Rotary encoder states
volatile int rotary_counter = 0;
volatile bool rotary_changed = false;

// Previous state of properties
bool prev_pad_state = false;
bool prev_polarity_state = false;
bool prev_hpf_state = false;
bool prev_phantom_power_state = false;
bool prev_input_impedance_state = false;
uint16_t prev_volume = 0;
bool prev_mute_state = false;

// CRC parameters
const uint8_t polynomial = 0x31; // CRC-8-CCITT polynomial

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

    // Initialize button pin
    pinMode(BUTTON_PIN, INPUT);

    // Initialize rotary encoder pins
    pinMode(ROTARY_PIN_A, INPUT_PULLUP);
    pinMode(ROTARY_PIN_B, INPUT_PULLUP);
    pinMode(ROTARY_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), rotaryEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), rotaryEncoderISR, CHANGE);

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    // Clear the display buffer.
    display.clearDisplay();
    display.display();
}

void loop() {
    // Read button state
    button_state = digitalRead(BUTTON_PIN);

    // Handle button press
    if (button_state != last_button_state) {
        if (button_state == HIGH) {
            // Button pressed
            toggleMute(); // Toggle mute state
        }
    }

    // Update OLED display
    updateDisplay();

    // Update last button state
    last_button_state = button_state;

    // Handle rotary encoder change
    if (rotary_changed) {
        int new_volume = map(rotary_counter, 0, ROTARY_ENCODER_STEPS, 0, 1023);
        if (new_volume != prev_volume) {
            setVolume(new_volume);
            sendVolumeUpdate(new_volume);
            prev_volume = new_volume;
        }
        rotary_changed = false;
    }

    // Your Arduino code here
}

void receiveEvent(int length) {
    if (length == sizeof(ControlData) + 1) { // Add 1 for CRC byte
        ControlData data;
        uint8_t data_with_crc[length];
        Wire.readBytes(data_with_crc, length);

        // Validate CRC
        if (validateCRC(data_with_crc, length)) {
            // CRC is valid, process data
            memcpy(&data, data_with_crc, sizeof(ControlData));

            // Set relays based on bool states
            for (int i = 0; i < NUM_RELAYS; ++i) {
                if (data.bool_states[i] != prev_bool_states[i]) {
                    digitalWrite(RELAY_PINS_START + i, data.bool_states[i]);
                    prev_bool_states[i] = data.bool_states[i];
                }
            }

            // Set volume if changed
            if (data.volume != prev_volume) {
                setVolume(data.volume);
                prev_volume = data.volume;
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

bool validateCRC(uint8_t* data_with_crc, int length) {
    // Validate CRC checksum for received data
    uint8_t crc = data_with_crc[length - 1];
    uint8_t calculated_crc = calculateCRC(data_with_crc, length - 1); // Exclude CRC byte
    return crc == calculated_crc;
}

uint8_t calculateCRC(uint8_t* data, size_t length) {
    // Calculate CRC checksum for data
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

void updateDisplay() {
    // Update OLED display with current states
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Mic Preamp Control");

    display.setCursor(0, 10);
    display.print("PAD: ");
    display.println(pad_state ? "On" : "Off");

    display.setCursor(0, 20);
    display.print("Polarity: ");
    display.println(polarity_state ? "On" : "Off");

    display.setCursor(0, 30);
    display.print("HPF: ");
    display.println(hpf_state ? "On" : "Off");

    display.setCursor(0, 40);
    display.print("Phantom Power: ");
    display.println(phantom_power_state ? "On" : "Off");

    display.setCursor(0, 50);
    display.print("Input Impedance: ");
    display.println(input_impedance_state ? "On" : "Off");

    display.display();
}

void toggleMute() {
    // Toggle mute state
    mute_state = !mute_state;
    // Send updated mute state
    ControlData data;
    data.device_id = readDipSwitches();
    data.mute = mute_state;
    sendData(&data);
}

void setVolume(uint16_t volume) {
    // Function to set volume
    // Your implementation here
}

void sendVolumeUpdate(uint16_t volume) {
    // Function to send volume update
    ControlData data;
    data.device_id = readDipSwitches();
    data.volume = volume;
    sendData(&data);
}

void sendData(ControlData* data) {
    // Send data over I2C
    Wire.beginTransmission(data->device_id);
    Wire.write((uint8_t*)data, sizeof(ControlData));
    Wire.endTransmission();
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

void rotaryEncoderISR() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 5ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 5) {
        if (digitalRead(ROTARY_PIN_A) == HIGH) {
            if (digitalRead(ROTARY_PIN_B) == LOW) {
                rotary_counter++;
            }
            else {
                rotary_counter--;
            }
        }
        else {
            if (digitalRead(ROTARY_PIN_B) == HIGH) {
                rotary_counter++;
            }
            else {
                rotary_counter--;
            }
        }
        rotary_changed = true;
    }
    last_interrupt_time = interrupt_time;
}
