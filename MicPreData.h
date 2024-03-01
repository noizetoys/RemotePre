
#include <EEPROM.h>


typedef struct MicPreData {
  unsigned char gainLevel;
  bool padEngaged;
  
  bool phantomEngaged;
  bool polarityInverted;
  bool inputZIsHigh;
  bool muteEngaged;
  bool highPassFilterEngaged;
  
  unsigned char deviceID;

  MicPreData() { }

  MicPreData(unsigned char id) {
    deviceID = id;
  }
  
} MicPreData;
