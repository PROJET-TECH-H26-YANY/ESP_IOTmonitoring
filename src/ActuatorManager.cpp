#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(int ledPin, int buzzerPin)
{
  _ledPin = ledPin;
  _buzzerPin = buzzerPin;
}

void ActuatorManager::begin()
{
  pinMode(_ledPin, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
  digitalWrite(_buzzerPin, LOW);
}

void ActuatorManager::processCommand(JsonDocument &doc)
{
  if (doc.containsKey("led"))
  {
    if (doc["led"] == 100)
      digitalWrite(_ledPin, HIGH);
    else if (doc["led"] == 0)
      digitalWrite(_ledPin, LOW);
    else if (doc["led"] == "blink_red")
      blinkError();
  }

  if (doc.containsKey("buzzer"))
  {
    if (doc["buzzer"] == true)
      digitalWrite(_buzzerPin, HIGH);
    else if (doc["buzzer"] == false)
      digitalWrite(_buzzerPin, LOW);
    else if (doc["buzzer"] == "short")
      shortBeep();
  }
}

void ActuatorManager::shortBeep()
{
  digitalWrite(_buzzerPin, HIGH);
  delay(100);
  digitalWrite(_buzzerPin, LOW);
}

void ActuatorManager::blinkError()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(_ledPin, HIGH);
    delay(150);
    digitalWrite(_ledPin, LOW);
    delay(150);
  }
}