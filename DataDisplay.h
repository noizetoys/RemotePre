
#ifndef DataDisplay_h
#define DataDisplay_h

#include "MicPreData.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



class DataDisplay {

  private:
    unsigned int gainLevels[12];
    //['25', '29', '33', '36', '39', '42', '45', '48', '51', '54', '57', '60'];


  private:
    //    void setGainLevel(int gainValue);
    //    void setPad(bool state);
    //    void set48v(bool state);
    //    void setPolarity(bool state);
    //    void setInputZ(bool state);
    //    void setMute(bool state);
    //    void setHPF(bool state);
    //    void setAudioLevel(int level);

//    void updateDataDisplay(MicPreData data);

    void displayMute();
    void displayReadout(MicPreData data);
    
  public:

    void displayData(MicPreData data);

    DataDisplay();
//    DataDisplay(MicPreData data);

};


#endif
