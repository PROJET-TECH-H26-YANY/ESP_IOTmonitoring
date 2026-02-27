#ifndef DISTANCE_MANAGER_H
#define DISTANCE_MANAGER_H

#include <Arduino.h>

class DistanceManager {
  private:
    int _rxPin, _txPin;
    HardwareSerial& _serialPort;

  public:
    DistanceManager(int rxPin, int txPin, HardwareSerial& serialPort = Serial2);
    void begin();
    float readDistanceCm(); 
};

#endif