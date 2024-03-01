
#include "DataDisplay.h"


class MicPreController {
    MicPreData data;
    DataDisplay dataDisplay;

    unsigned int deviceID;
    

  public:
    void setGainLevel(int gainValue);
    void padTouched(bool state);
    void phantomTouched(bool state);
    void polarityTouched(bool state);
    void inputZTouched(bool state);
    void muteTouched(bool state);
    void highPassTouched(bool state);
    //    void setAudioLevel(int level);


    MicPreController() {
      deviceID = 33;
    }

    MicPreController(unsigned int id) {
      deviceID = id;
    }

};
