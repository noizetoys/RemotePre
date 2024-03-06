// To enhance the provided code with functionality for setting the device ID using a PISO shift register, handling rotary encoder changes, and tracking all state changes for the buttons and `ControlData`, we'll proceed as follows:

// 1. **Implement Device ID Setting**: Utilize the PISO shift register to set the device ID.
// 2. **Handle Rotary Encoder Changes**: Update the rotary encoder ISR to properly handle changes and update the volume in `ControlData`.
// 3. **Track Button and `ControlData` State Changes**: Implement logic to detect state changes in buttons and update `ControlData` accordingly.

// ### Final Code with Enhancements


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
#define BUTTON_PIN 3
#define ROTARY_PIN_A 4
#define ROTARY_PIN_B 5
#define ROTARY_BUTTON_PIN 6
#define ROTARY_ENCODER_STEPS 12
#define SHIFT_REGISTER_DATA_PIN A0
#define SHIFT_REGISTER_CLOCK_PIN A1
#define SHIFT_REGISTER_LATCH_PIN A2
#define NUM_DIP_SWITCHES 5

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Control data struct
struct ControlData {
    uint8_t device_id;
    uint16_t volume;
    bool mute;
    bool pad_state;
    bool polarity_state;
    bool hpf_state;
    bool phantom_power_state;
    bool input_impedance_state;

    // Constructor to initialize with default values
    ControlData() : device_id(0), volume(0), mute(false), pad_state(false), polarity_state(false), hpf_state(false), phantom_power_state(false), input_impedance_state(false) {}
};

ControlData controlData;

// Button and Rotary encoder states
bool button_state = false;
bool last_button_state = false;
volatile int rotary_counter = 0;
volatile bool rotary_changed = false;

// CRC parameters
const uint8_t polynomial = 0x31; // CRC-8-CCITT polynomial



class DisplayHandler {
public:
    Adafruit_SSD1306 display;

    DisplayHandler() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    void init() {
        // Initialize the OLED display
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C is the I2C address for the display
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);  // Don't proceed, loop forever
        }
        display.clearDisplay();  // Clear the buffer
        display.display();       // Show the cleared buffer on the display
    }

    void updateDisplay(const ControlData& data) {
        display.clearDisplay();  // Clear the display buffer
        display.setTextSize(1);  // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);  // Draw white text
        display.setCursor(0, 0);  // Start at top-left corner

        // Displaying the volume and mute status
        display.println(F("Volume:"));
        display.println(data.volume);
        display.println(F("Mute:"));
        display.println(data.mute ? F("On") : F("Off"));

        // Displaying additional properties
        display.println(F("Pad:"));
        display.println(data.pad_state ? F("On") : F("Off"));
        display.println(F("Polarity:"));
        display.println(data.polarity_state ? F("Inv") : F("Norm"));
        display.println(F("HPF:"));
        display.println(data.hpf_state ? F("On") : F("Off"));
        display.println(F("Phantom:"));
        display.println(data.phantom_power_state ? F("On") : F("Off"));
        display.println(F("Impedance:"));
        display.println(data.input_impedance_state ? F("Hi") : F("Lo"));

        display.display();  // Show the contents of the display buffer on the OLED
    }
};

class I2CCommunication {
public:
    void init(uint8_t device_id) {
        Wire.begin(device_id);
        Wire.onReceive(receiveEvent);
    }

    static void receiveEvent(int length) {
        if (length == sizeof(ControlData) + 1) { // Add 1 for CRC byte
            ControlData data;
            uint8_t data_with_crc[length];
            Wire.readBytes(data_with_crc, length);

            if (validateCRC(data_with_crc, length)) {
                memcpy(&data, data_with_crc, sizeof(ControlData));
                // Process data...
                controlData = data; // Update the global controlData with the new settings
                displayHandler.updateDisplay(controlData); // Update display with new data
            } else {
                Serial.println("CRC validation failed");
            }
        } else {
            while (Wire.available()) Wire.read();
            Serial.println("Invalid data received");
        }
    }

    static bool validateCRC(uint8_t* data_with_crc, int length) {
        uint8_t crc = data_with_crc[length - 1];
        uint8_t calculated_crc = calculateCRC(data_with_crc, length - 1);
        return crc == calculated_crc;
    }

    static uint8_t calculateCRC(uint8_t* data, size_t length) {
        uint8_t crc = 0xFF;
        for (size_t i = 0; i < length; ++i) {
            crc ^= data[i];
            for

 (uint8_t bit = 0; bit < 8; ++bit) {
                if (crc & 0x80) crc = (crc << 1) ^ polynomial;
                else crc <<= 1;
            }
        }
        return crc;
    }
};

DisplayHandler displayHandler;
I2CCommunication i2cCommunication;

void setup() {
    Serial.begin(9600);
    controlData.device_id = readDipSwitches();
    i2cCommunication.init(controlData.device_id);
    displayHandler.init();

    pinMode(BUTTON_PIN, INPUT);
    pinMode(ROTARY_PIN_A, INPUT_PULLUP);
    pinMode(ROTARY_PIN_B, INPUT_PULLUP);
    pinMode(ROTARY_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), rotaryEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), rotaryEncoderISR, CHANGE);
}

void loop() {
    // Button state reading and handling
    button_state = digitalRead(BUTTON_PIN);
    if (button_state != last_button_state) {
        if (button_state == HIGH) {
            // Button pressed
            controlData.mute = !controlData.mute; // Toggle mute state
            displayHandler.updateDisplay(controlData); // Update display
        }
        last_button_state = button_state;
    }

    // Handle rotary encoder change
    if (rotary_changed) {
        controlData.volume = map(rotary_counter, 0, ROTARY_ENCODER_STEPS, 0, 100); // Map rotary_counter to volume range
        displayHandler.updateDisplay(controlData); // Update display
        rotary_changed = false;
    }
}

void rotaryEncoderISR() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // Debounce by ignoring interrupts coming faster than 5ms
    if (interrupt_time - last_interrupt_time > 5) {
        bool newA = digitalRead(ROTARY_PIN_A);
        bool newB = digitalRead(ROTARY_PIN_B);

        if (newA != newB) {
            rotary_counter++;
        } else {
            rotary_counter--;
        }
        rotary_changed = true;
    }
    last_interrupt_time = interrupt_time;
}

uint8_t readDipSwitches() {
    uint8_t id = 0;
    digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
    delayMicroseconds(5);  // Allow time for the latch to settle
    digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);

    for (int i = 0; i < NUM_DIP_SWITCHES; ++i) {
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);
        delayMicroseconds(5);  // Allow time for the data to be read
        id |= digitalRead(SHIFT_REGISTER_DATA_PIN) << i;
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, LOW);
    }
    return id;
}
```

// ### Key Enhancements:

// - **Device ID with PISO Shift Register**: The `readDipSwitches` function reads the DIP switch settings from a PISO shift register to set the device ID. This ID is then used to initialize the I2C communication.

// - **Rotary Encoder Handling**: The `rotaryEncoderISR` function has been updated to properly interpret the A and B signals from the rotary encoder to increment or decrement the `rotary_counter`. This change is reflected in the `ControlData.volume` property, and the display is updated accordingly.

// - **Button State Tracking**: Button presses toggle the mute state in the `ControlData` and update the display. The last button state is tracked to detect changes.

// - **Display and I2C Updates**: The `DisplayHandler` and `I2CCommunication` classes have been updated to handle the new `ControlData` properties. When I2C data is received and validated, it updates the global `controlData` and subsequently the display.

// This comprehensive update ensures that the device can set its ID, process rotary encoder input, track button presses, and handle I2C communication effectively, with all state changes reflected on the OLED display.