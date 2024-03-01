
//#ifndef

class DataDisplay(

/* Display Items
 * Gain Level
 * 'db' marking
 * Pad
 * 48V
 * Polarity
 * Input Z
 * Mute
 * HPF
 * Device ID
 * Audio Level
 * 
  */

 private:
  int gainLevel;
  bool padEngaged;
  bool phantomEngaged;
  

 
void setGainLevel(int gainValue);
void setPad(bool state);
void set48v(bool state);
void setPolarity(bool state);
void setInputZ(bool state);
void setMute(bool state);
void setHPF(bool state);
void setAudioLevel(bool state);


  )
  

//#endif
