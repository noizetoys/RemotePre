#include "GainEncoder.h"
#include <Arduino.h>



GainEncoder::GainEncoder() {
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);

//  attachInterrupt(0, GainEncoder::readEncoder, CHANGE);
}


// Public

int GainEncoder::readEncoder() {
  AValue = digitalRead(encoder0PinA);
  BValue = digitalRead(encoder0PinB);

  if (encoder0Pos >= EncoderMin && encoder0Pos <= EncoderMax) {
    AValue == BValue ? encoder0Pos++ : encoder0Pos--;
  }

  //  Serial.print("doEncoder:  Current Encoder Value = ");
  //  Serial.println(encoder0Pos);

  newRotationStep = (encoder0Pos / EncoderStep);

  if (newRotationStep != rotationStep && encoder0Pos >= EncoderStep) {
    rotationStep = newRotationStep;
    Serial.print("\nRotationStep = ");
    Serial.println(rotationStep);
  }

  if (encoder0Pos < EncoderMin) {
    Serial.print(EncoderMin);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMin;
    rotationStep = EncoderMin;
  }
  else if (encoder0Pos > EncoderMax) {
    Serial.print(EncoderMax);
    Serial.println(" Hit, No Change");
    encoder0Pos = EncoderMax;
    rotationStep = EncoderMax / EncoderStep;
  }

  return rotationStep;
}
