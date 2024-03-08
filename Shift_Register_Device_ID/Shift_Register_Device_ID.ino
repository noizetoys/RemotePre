
#include "Config.h"
#include "Rotary.h"


int deviceID;

byte button_pressed;
byte last_button_pressed;

int EncoderMin = 1;
int EncoderMax = (8 * 12);
int EncoderStep = 8;

volatile int encoder0Pos = 1;
volatile int gainLevel = 0;
volatile int AValue;
volatile int BValue;
volatile int rotationStep, newRotationStep, btn;

void show_button_update(byte button, bool state) {
  Serial.print(F("show_button_update called:  "));
  Serial.print(F("button #  "));
  Serial.print(button);
  Serial.print(F(" = "));
  Serial.println(state);
}


typedef struct ButtonStates {
  const int button_one = 1;
  const int button_two = 2;
  const int button_three = 4;
  const int button_four = 8;
  const int encoder_button = 16;

  bool button_one_pressed = false;
  bool button_two_pressed = false;
  bool button_three_pressed = false;
  bool button_four_pressed = false;
  bool encoder_button_pressed = false;

  void update_button(byte button) {
    int int_button = int(button);

    switch (int_button) {
      case 1:
        button_one_pressed = !button_one_pressed;
        show_button_update(button, button_one_pressed);
        break;

      case 2:
        button_two_pressed = !button_two_pressed;
        show_button_update(button, button_two_pressed);
        break;

      case 4:
        button_three_pressed = !button_three_pressed;
        show_button_update(button, button_three_pressed);
        break;

      case 8:
        button_four_pressed = !button_four_pressed;
        show_button_update(button, button_four_pressed);
        break;

      case 16:
        encoder_button_pressed = !encoder_button_pressed;
        show_button_update(button, encoder_button_pressed);

      default:
        break;
    }
  }

} ButtonStates;


ButtonStates button_states = ButtonStates();

/*
   SETUP
*/
void setup() {
  Serial.begin(115200);
  Serial.println(F("\n--> Setup"));

  configure_encoder();
  configure_Button_Pins();
  configure_Device_ID_Pins();

  // Needs to be done twice to remove stale data
  deviceID = load_device_ID();
  deviceID = load_device_ID();

  Serial.print(F("setup -> Device ID:  "));
  Serial.println(deviceID);

  Serial.println(F("<-- Setup Complete"));
}


void loop() {
  monitor_buttons();

    delay(250);
}


void configure_encoder() {
  pinMode(ROTARY_ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_PIN_B, INPUT_PULLUP );
  //  pinMode(ROTARY_ENCODER_PIN_A, INPUT);
  //  pinMode(ROTARY_ENCODER_PIN_B, INPUT);
  attachInterrupt(0, doEncoder, CHANGE);
}

void configure_Button_Pins() {
  pinMode(BUTTONS_DATA_PIN, INPUT);

  pinMode(BUTTONS_CLOCK_PIN, OUTPUT);
  pinMode(BUTTONS_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(BUTTONS_LATCH_PIN, OUTPUT);

  digitalWrite(BUTTONS_CLOCK_ENABLE_PIN, LOW);
  delayMicroseconds(5);
}

void configure_Device_ID_Pins() {
  pinMode(DEVICE_ID_DATA_PIN, INPUT);

  pinMode(DEVICE_ID_CLOCK_PIN, OUTPUT);
  pinMode(DEVICE_ID_CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(DEVICE_ID_LATCH_PIN, OUTPUT);

  digitalWrite(DEVICE_ID_CLOCK_ENABLE_PIN, LOW);
  delayMicroseconds(5);
}


// Pulse Latch (Read) Pins
void pulse_latch(int latch_pin) {
  digitalWrite(latch_pin, LOW);
  delayMicroseconds(5);

  digitalWrite(latch_pin, HIGH);
  delayMicroseconds(5);
}


int load_device_ID() {
  int id;

  pulse_latch(DEVICE_ID_LATCH_PIN);

  // Enable Clock
  digitalWrite(DEVICE_ID_CLOCK_PIN, HIGH);
  digitalWrite(DEVICE_ID_CLOCK_ENABLE_PIN, LOW);

  id = shiftIn(DEVICE_ID_DATA_PIN, DEVICE_ID_CLOCK_PIN, MSBFIRST);

  //  Serial.print(F("MSBFIRST:  Binary = "));
  //  Serial.print(id, BIN);
  //  Serial.print(F(", Int = "));
  //  Serial.println(int(id));

  // Disable Clock
  digitalWrite(DEVICE_ID_CLOCK_ENABLE_PIN, HIGH);

  return id;
}


void monitor_buttons() {
  pulse_latch(BUTTONS_LATCH_PIN);

  // Enable Clock
  digitalWrite(BUTTONS_CLOCK_PIN, HIGH);
  digitalWrite(BUTTONS_CLOCK_ENABLE_PIN, LOW);

  button_pressed = shiftIn(BUTTONS_DATA_PIN, BUTTONS_CLOCK_PIN, MSBFIRST);

  if (button_pressed > 0) {// && button_pressed != last_button_pressed) {
    //    Serial.print(F("MSBFIRST:  Binary = "));
    //    Serial.print(button_pressed, BIN);
    //    Serial.print(F(", Int = "));
    //    Serial.println(int(button_pressed));
    last_button_pressed = button_pressed;
    button_states.update_button(button_pressed);
  }

  // Disable Clock
  digitalWrite(BUTTONS_CLOCK_ENABLE_PIN, HIGH);
}


void updateGainLevel(int AValue, int BValue) {
  Serial.print(F("\n~~ MicPreControl: updateGainLevel() called!"));
  Serial.print(F(" --> AValue: "));
  Serial.print(AValue);
  Serial.print(F(", BValue: "));
  Serial.println(BValue);

  //  Serial.print(F("MicPreStateProcessor::updateGainLevel:  Current Encoder Value = "));

  if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
    AValue == BValue ? encoder0Pos++ : encoder0Pos--;
  }

  newRotationStep = (encoder0Pos / EncoderStep);

  if (newRotationStep != rotationStep && encoder0Pos >= EncoderStep) {
    rotationStep = newRotationStep;
    Serial.print(F("\nRotationStep = "));
    Serial.println(rotationStep);
  }

  if (encoder0Pos < EncoderMin) {
    Serial.print(EncoderMin);
    Serial.println(F(" Hit, No Change"));
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
    Serial.print(EncoderMax);
    Serial.println(F(" Hit, No Change"));
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

  if (gainLevel != rotationStep) {
    Serial.print(F("Gain Level Changed:  "));
    Serial.println(rotationStep);
    gainLevel = rotationStep;
  }

  Serial.print(F("\n~~ gainLevel = "));
  Serial.println(gainLevel);
}


int a_pin;
int b_pin;


void doEncoder() {
  a_pin = digitalRead(ROTARY_ENCODER_PIN_A);
  b_pin = digitalRead(ROTARY_ENCODER_PIN_B);

  if (a_pin == 0 || b_pin == 0) {
    Serial.println(F("--> doEncoder called"));
    Serial.print(F("* A Pin == "));
    Serial.print(a_pin);
    Serial.print(F(", * B Pin == "));
    Serial.println(b_pin);

    updateGainLevel(a_pin, b_pin);
  }
}
