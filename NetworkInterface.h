#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H


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
