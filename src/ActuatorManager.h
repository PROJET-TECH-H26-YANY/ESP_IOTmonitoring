#ifndef ACTUATOR_MANAGER_H
#define ACTUATOR_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

class ActuatorManager {
  private:
    int _ledPin, _buzzerPin;

  public:
    ActuatorManager(int ledPin, int buzzerPin);
    void begin();
    void processCommand(JsonDocument& doc); 
    void shortBeep();                       
    void blinkError();                      
};

#endif