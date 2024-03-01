

/*
   Data sent to Display & Network
*/
typedef struct MicPreData {
  unsigned int gainLevel;

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

  unsigned int deviceID;

  MicPreData() { }

  MicPreData(int id) {
    deviceID = id;
  }

  void resetData() {
    padEngaged = false;
    phantomEngaged = false;
    polarityInverted = false;
    inputZIsHigh = false;
    muteEngaged = false;
    highPassFilterEngaged = false;

    padButtonState = false;
    phantomButtonState = false;
    polarityButtonState = false;
    inputZButtonState = false;
    muteButtonState = false;
    highPassFilterButtonState = false;
  }

} MicPreData;
