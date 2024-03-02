#ifndef NetworkInterface_h
#define NetworkInterface_h


#include <Wire.h>


class NetworkInterface {
    char deviceID;


  public:
    NetworkInterface() { }

    NetworkInterface(char id) {
      deviceID = id;
    }
};


#endif
