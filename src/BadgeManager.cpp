// BadgeManager.cpp
#include "BadgeManager.h"
#include <Arduino.h>
#include "MqttManager.h"
#include "RfidManager.h"      // Remplace par ton vrai fichier RFID
#include "ActuatorManager.h"  // Remplace par ton vrai fichier Actuators

extern MqttManager mqtt;
extern RfidManager rfid;      // Type à adapter selon ta classe RFID
extern ActuatorManager actuators;

const unsigned long SCAN_COOLDOWN = 2000;
unsigned long lastScanTime = 0;

void handleBadge() {
  unsigned long currentTime = millis();
  if (currentTime - lastScanTime >= SCAN_COOLDOWN) {
    String badgeUid = rfid.readBadge();
    
    if (badgeUid.length() > 0) { // OPTIMISATION
      lastScanTime = currentTime;
      String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"nfc\": \"" + badgeUid + "\"}";
      mqtt.publish("labo/device/auth", payload);
      actuators.shortBeep();
    }
  }
}