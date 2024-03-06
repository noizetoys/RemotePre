// ControlArduino.ino

#include <Wire.h>
#include <Rotary.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"
#include "Logger.h"
#include "ControlState.h"
#include "StateManager.h"
#include "DataDisplay.h"

Rotary rotary(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B);
StateManager stateManager;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DataDisplay dataDisplay(display);

uint8_t i2cAddress;

// Button state machine
enum ButtonState {
    BUTTON_RELEASED,
    BUTTON_PRESSED,
    BUTTON_DEBOUNCED
};

ButtonState phaseButtonState = BUTTON_RELEASED;
ButtonState phantomPowerButtonState = BUTTON_RELEASED;
ButtonState inputImpedanceButtonState = BUTTON_RELEASED;
ButtonState padButtonState = BUTTON_RELEASED;
ButtonState highPassFilterButtonState = BUTTON_RELEASED;

uint8_t debounceIterations = 0;

// I2C protocol
const uint8_t I2C_START_BYTE = 0xAA;
const uint8_t I2C_END_BYTE = 0xBB;
const uint8_t I2C_BUFFER_SIZE = 32;
uint8_t i2cTxBuffer[I2C_BUFFER_SIZE];
uint8_t i2cRxBuffer[I2C_BUFFER_SIZE];
uint8_t i2cTxBufferIndex = 0;
uint8_t i2cRxBufferIndex = 0;

void setup() {
    Logger::init();
    Logger::info("Control Arduino starting...");

    i2cAddress = readDIPSwitches();
    Wire.begin(i2cAddress);
    Logger::debug("I2C address set");

    pinMode(ROTARY_ENCODER_SWITCH_PIN, INPUT_PULLUP);
    pinMode(PHASE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(PHANTOM_POWER_BUTTON_PIN, INPUT_PULLUP);
    pinMode(INPUT_IMPEDANCE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(PAD_BUTTON_PIN, INPUT_PULLUP);
    pinMode(HIGH_PASS_FILTER_BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_A), handleRotaryEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_PIN_B), handleRotaryEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_SWITCH_PIN), handleMute, FALLING);

    Wire.onReceive(receiveControlStates);
    Wire.onRequest(sendControlStates);

    dataDisplay.updateDisplay(stateManager.getControlState());

    Logger::info("Control Arduino setup completed");
}

void loop() {
    checkAndSignalStateChanges();
    debounceButtons();
}

void handleRotaryEncoder() {
    unsigned char result = rotary.process();
    if (result == DIR_CW) {
        int newGainState = (stateManager.getControlState().gainLevel + 1) % NUM_GAIN_VALUES;
        stateManager.updateGainState(newGainState);
        dataDisplay.updateDisplay(stateManager.getControlState());
    } else if (result == DIR_CCW) {
        int newGainState = (stateManager.getControlState().gainLevel - 1 + NUM_GAIN_VALUES) % NUM_GAIN_VALUES;
        stateManager.updateGainState(newGainState);
        dataDisplay.updateDisplay(stateManager.getControlState());
    }
}

void handleMute() {
    stateManager.togglePhaseState();
    dataDisplay.updateDisplay(stateManager.getControlState());
}

void handlePhase() {
    phaseButtonState = BUTTON_PRESSED;
}

void handlePhantomPower() {
    phantomPowerButtonState = BUTTON_PRESSED;
}

void handleInputImpedance() {
    inputImpedanceButtonState = BUTTON_PRESSED;
}

void handlePad() {
    padButtonState = BUTTON_PRESSED;
}

void handleHighPassFilter() {
    highPassFilterButtonState = BUTTON_PRESSED;
}

uint8_t readDIPSwitches() {
    uint8_t address = I2C_BASE_ADDRESS;
    for (uint8_t i = 0; i < NUM_DIP_SWITCH_PINS; i++) {
        int pinValue = digitalRead(DIP_SWITCH_PIN_START + i);
        address |= (pinValue << i);
    }
    return address;
}

void checkAndSignalStateChanges() {
    if (stateManager.isStateChanged()) {
        digitalWrite(MULTIPLEXER_CONTROL_PIN, HIGH);
        delay(10);
        digitalWrite(MULTIPLEXER_CONTROL_PIN, LOW);
        stateManager.resetStateChanged();
    }
}

void debounceButtons() {
    if (phaseButtonState == BUTTON_PRESSED && digitalRead(PHASE_BUTTON_PIN) == LOW) {
        debounceIterations++;
        if (debounceIterations >= DEBOUNCE_ITERATIONS) {
            phaseButtonState = BUTTON_DEBOUNCED;
            stateManager.togglePhaseState();
            dataDisplay.updateDisplay(stateManager.getControlState());
            debounceIterations = 0;
        }
    } else if (phaseButtonState == BUTTON_DEBOUNCED && digitalRead(PHASE_BUTTON_PIN) == HIGH) {
        phaseButtonState = BUTTON_RELEASED;
    }

    // Similar debounce logic for other buttons...
}

void sendControlStates() {
    ControlState state = stateManager.getControlState();
    i2cTxBuffer[0] = I2C_START_BYTE;
    i2cTxBuffer[1] = state.gainLevel;
    i2cTxBuffer[2] = state.phaseState;
    i2cTxBuffer[3] = state.phantomPowerState;
    i2cTxBuffer[4] = state.inputImpedanceState;
    i2cTxBuffer[5] = state.padState;
    i2cTxBuffer[6] = state.highPassFilterState;
    i2cTxBuffer[7] = I2C_END_BYTE;
    i2cTxBufferIndex = 0;
    Wire.write(i2cTxBuffer[i2cTxBufferIndex++]);
}

void receiveControlStates(int byteCount) {
    while (Wire.available()) {
        uint8_t data = Wire.read();
        if (data == I2C_START_BYTE) {
            i2cRxBufferIndex = 0;
        } else if (data == I2C_END_BYTE) {
            ControlState newState;
            newState.gainLevel = i2cRxBuffer[0];
            newState.phaseState = i2cRxBuffer[1];
            newState.phantomPowerState = i2cRxBuffer[2];
            newState.inputImpedanceState = i2cRxBuffer[3];
            newState.padState = i2cRxBuffer[4];
            newState.highPassFilterState = i2cRxBuffer[5];

            if (newState != stateManager.getControlState()) {
                stateManager.updateGainState(newState.gainLevel);
                stateManager.togglePhaseState();
                stateManager.togglePhantomPowerState();
                stateManager.toggleInputImpedanceState();
                stateManager.togglePadState();
                stateManager.toggleHighPassFilterState();
                dataDisplay.updateDisplay(stateManager.getControlState());
            }
        } else {
            i2cRxBuffer[i2cRxBufferIndex++] = data;
        }
    }
}