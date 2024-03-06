
// Control_Arduion_7

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin (or -1 if sharing Arduino reset pin)

// Pin Definitions
#define SIGNAL_PIN 2  // Pin used to signal the RPi
#define ROTARY_PIN_A 4
#define ROTARY_PIN_B 5
#define BUTTON_PIN 3

// Debounce Settings
#define DEBOUNCE_DELAY 50  // Button debounce time in milliseconds

// I2C Address Configuration
#define NUM_DIP_SWITCHES 5  // Assuming 5 DIP switches for address configuration
#define SHIFT_REGISTER_DATA_PIN A0
#define SHIFT_REGISTER_CLOCK_PIN A1
#define SHIFT_REGISTER_LATCH_PIN A2

// Control Data Structure with Constructor
struct ControlData {
    uint8_t device_id;
    uint16_t volume;
    bool mute;
    bool pad_state;
    bool polarity_state;
    bool hpf_state;
    bool phantom_power_state;
    bool input_impedance_state;

    ControlData() : device_id(0), volume(0), mute(false), pad_state(false),
                    polarity_state(false), hpf_state(false), phantom_power_state(false),
                    input_impedance_state(false) {}
} controlData, lastControlData;

// Function Prototypes
void onRequest();
void signalDataRequest();
void rotaryEncoderISR();
uint8_t readDipSwitches();
void updateControlData();

class DisplayHandler {
public:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void init() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);  // Don't proceed, loop forever
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.display();
    }

    void updateDisplay(const ControlData& data) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Volume: " + String(data.volume));
        display.println("Mute: " + String(data.mute ? "On" : "Off"));
        // Add more display lines for additional control states
        display.display();
    }
} displayHandler;

void setup() {
    Serial.begin(9600);
    Wire.begin();  // Join I2C bus

    // Initialize components
    displayHandler.init();
    pinMode(SIGNAL_PIN, OUTPUT);
    digitalWrite(SIGNAL_PIN, LOW);

    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Initialize Button Pin with Pull-up
    pinMode(ROTARY_PIN_A, INPUT_PULLUP);  // Initialize Rotary Encoder Pins with Pull-up
    pinMode(ROTARY_PIN_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), rotaryEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), rotaryEncoderISR, CHANGE);

    // Set device ID based on DIP switches and start I2C
    controlData.device_id = readDipSwitches();
    Wire.onRequest(onRequest);
}

void loop() {
    static unsigned long lastButtonPressTime = 0;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    static bool lastButtonState = HIGH;

    // Button Debouncing
    if (currentButtonState != lastButtonState && (millis() - lastButtonPressTime) > DEBOUNCE_DELAY) {
        if (currentButtonState == LOW) {
            controlData.mute = !controlData.mute;  // Toggle mute state
            signalDataRequest();  // Signal the RPi for data request
            lastButtonPressTime = millis();
        }
    }
    lastButtonState = currentButtonState;

    updateControlData();  // Update control data based on inputs
    displayHandler.updateDisplay(controlData);  // Update display with current control data

    // Send data if state changed
    if (memcmp(&controlData, &lastControlData, sizeof(ControlData)) != 0) {
        signalDataRequest();  // Signal the RPi
        lastControlData = controlData;  // Update the last known state
    }
}

void signalDataRequest() {
    digitalWrite(SIGNAL_PIN, HIGH);
    delay(10);  // Ensure RPi detects the signal
    digitalWrite(SIGNAL_PIN, LOW);
}

void onRequest() {
    // Send data only if there's a change
    if (memcmp(&controlData, &lastControlData, sizeof(ControlData)) != 0) {
        Wire.write((const uint8_t*)&controlData, sizeof(ControlData));
    }
}

void rotaryEncoderISR() {
    // Implement rotary encoder logic to update controlData.volume
    static bool lastA = LOW;
    bool currentA = digitalRead(ROTARY_PIN_A);
    if (currentA != lastA) {
        if (digitalRead(ROTARY_PIN_B) != currentA) {
            controlData.volume = min(controlData.volume + 1, 100);  // Increment volume with upper limit
        } else {
            controlData.volume = max(controlData.volume - 1, 0);  // Decrement volume with lower limit
        }
    }
    lastA = currentA;
}

uint8_t readDipSwitches() {
    uint8_t device_id = 0;
    pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_DATA_PIN, INPUT);

    digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
    delayMicroseconds(5);

    for (int i = 0; i < NUM_DIP_SWITCHES; i++) {
        device_id |= digitalRead(SHIFT_REGISTER_DATA_PIN) << i;
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(SHIFT_REGISTER_CLOCK_PIN, LOW);
        delayMicroseconds(5);
    }
    return device_id;
}

void updateControlData() {
    // Static variables to store the last states for comparison
    static bool lastButtonState = HIGH;
    static int lastRotaryPosition = 0;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    
    // Check for button state change to toggle mute
    if (currentButtonState != lastButtonState && currentButtonState == LOW) {
        controlData.mute = !controlData.mute;  // Toggle the mute state
        lastButtonState = currentButtonState;  // Update the last button state
    }

    // Update lastButtonState at the end of the loop iteration
    lastButtonState = currentButtonState;

    // Check for rotary encoder change to update volume
    // Note: Implementing a rotary encoder requires handling interrupts or polling its pins to determine direction and step count. 
    // The example below is simplified and assumes a function or mechanism (e.g., an interrupt service routine) updates 'rotaryCounter'.
    extern volatile int rotaryCounter;  // Assume this is defined and updated elsewhere, e.g., in an ISR
    if (rotaryCounter != lastRotaryPosition) {
        // Update volume based on rotaryCounter; implement the mapping logic as needed
        controlData.volume = map(rotaryCounter, 0, ROTARY_ENCODER_STEPS, 0, 100); // Example mapping
        lastRotaryPosition = rotaryCounter;  // Update the last rotary position
    }
}
