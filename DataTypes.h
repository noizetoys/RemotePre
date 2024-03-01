

/*
   Data sent to Display & Network
*/
typedef struct MicPreData {
  unsigned char gainLevel;
  bool padEngaged;

  bool phantomEngaged;
  bool polarityInverted;
  bool inputZIsHigh;
  bool muteEngaged;
  bool highPassFilterEngaged;

  // Needed to keep local and remote interfaces in sync
  bool padButtonState;
  bool phantomButtonState;
  bool polarityButtonState;
  bool inputZButtonState;
  bool muteButtonState;
  bool highPassFilterButtonState;

  unsigned char deviceID;

  MicPreData() { }

  MicPreData(unsigned char id) {
    deviceID = id;

    // Button State
//    padButtonState = false;
//    phantomButtonState = false;
//    polarityButtonState = false;
//    inputZButtonState = false;
//    muteButtonState = false;
//    highPassFilterButtonState = false;

    // FUNCTION STATE
//    gainLevel = 0;
//    padEngaged = false;
//
//    phantomEngaged = false;
//    polarityInverted = false;
//    inputZIsHigh = false;
//    muteEngaged = false;
//    highPassFilterEngaged = false;

  }

} MicPreData;


enum MicPreButton {
  pad = 2,
  phantom = 3,
  polarity = 4,
  inputZ = 5,
  mute = 6,
  highPassFilter = 7,
};
