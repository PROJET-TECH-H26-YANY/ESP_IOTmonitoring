#ifndef ACTUATOR_MANAGER_H
#define ACTUATOR_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

class ActuatorManager {
  private:
    int _pinLed;
    int _pinBuzzer;

  public:
    ActuatorManager(int pinLed, int pinBuzzer);

    void begin();

    void traiterCommandeJSON(JsonDocument& doc);

    void clignoterErreur();
    void bipCourt();
};

#endif