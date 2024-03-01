

//union InputZ {
//  bool low;
//  bool hi;
//} inputZ;

#include "DataTypes.h"


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
    void setAudioLevel(int level);

    void updateDataDisplay(MicPreData data);


  public:

    void setData(MicPreData data);

    DataDisplay() { }


    //    DataDisplay(MicPreData data) {
    //    }

};
