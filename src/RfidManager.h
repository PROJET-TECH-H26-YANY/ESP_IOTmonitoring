#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <SPI.h>
#include <MFRC522.h>

class RfidManager {
  private:
    MFRC522* _mfrc522;
    int _ssPin, _rstPin;

  public:
    RfidManager(int ssPin, int rstPin);
    void begin();
    String readBadge();   
};

#endif