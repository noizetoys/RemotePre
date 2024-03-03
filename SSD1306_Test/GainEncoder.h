#ifndef GAINENCODER_H
#define GAINENCODER_H



/*
   Encoder
*/
#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4

#define EncoderMin 1
#define EncoderMax 120
#define EncoderStep 8

class GainEncoder  {
  private: 
    volatile int encoder0Pos = 0;
    volatile int AValue;
    volatile int BValue;
    volatile int rotationStep, newRotationStep, btn;

  public:
    int readEncoder();

    GainEncoder();

};

#endif
