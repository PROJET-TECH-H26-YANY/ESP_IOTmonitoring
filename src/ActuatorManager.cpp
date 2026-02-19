#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(int pinLed, int pinBuzzer) {
  _pinLed = pinLed;
  _pinBuzzer = pinBuzzer;
}

void ActuatorManager::begin() {
  pinMode(_pinLed, OUTPUT);
  pinMode(_pinBuzzer, OUTPUT);
  
  digitalWrite(_pinLed, LOW);
  digitalWrite(_pinBuzzer, LOW);
}

// Traitement des ordres venus du MQTT (Node-RED)
void ActuatorManager::traiterCommandeJSON(JsonDocument& doc) {
  
  if (doc.containsKey("led")) {
    if (doc["led"] == 100) {
      digitalWrite(_pinLed, HIGH);
    } 
    else if (doc["led"] == 0) {
      digitalWrite(_pinLed, LOW);
    } 
    else if (doc["led"] == "blink_red") {
      clignoterErreur();
    }
  }

  if (doc.containsKey("buzzer")) {
    if (doc["buzzer"] == true) {
      digitalWrite(_pinBuzzer, HIGH);
    } 
    else if (doc["buzzer"] == false) {
      digitalWrite(_pinBuzzer, LOW);
    } 
    else if (doc["buzzer"] == "short") {
      bipCourt();
    }
  }
}

void ActuatorManager::clignoterErreur() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(_pinLed, HIGH); 
    delay(150);
    digitalWrite(_pinLed, LOW);  
    delay(150);
  }
}

void ActuatorManager::bipCourt() {
  digitalWrite(_pinBuzzer, HIGH); 
  delay(100); 
  digitalWrite(_pinBuzzer, LOW);
}