// TelemetryManager.cpp
#include "TelemetryManager.h"
#include <Arduino.h>
#include "MqttManager.h"
#include "DisplayManager.h"
extern MqttManager mqtt;
extern DisplayManager display;
extern DistanceManager ultrasonic;
extern bool isSessionActive;

const unsigned long TELEMETRY_INTERVAL = 1000;
unsigned long lastTelemetryTime = 0;

void handleTelemetry() {
  if (isSessionActive) {
    unsigned long currentTime = millis();
    if (currentTime - lastTelemetryTime >= TELEMETRY_INTERVAL) {
      lastTelemetryTime = currentTime;
      float distance = ultrasonic.readDistanceCm();
      if (distance != -1.0) {
        String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"distance\": " + String(distance) + "}";
        mqtt.publish("labo/device/telemetry", payload);
      }
    }
  }
}