#include <Arduino.h>
#include <ArduinoJson.h>
#include "Secrets.h"      
#include "MqttManager.h"
#include "RfidManager.h"
#include "ActuatorManager.h"
#include "DistanceManager.h"

const int RFID_SS_PIN = 5;
const int RFID_RST_PIN = 4;
const int LED_PIN = 25;
const int BUZZER_PIN = 26;
const int ULTRA_RX_PIN = 16; 
const int ULTRA_TX_PIN = 17; 

MqttManager mqtt(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
RfidManager rfid(RFID_SS_PIN, RFID_RST_PIN);
ActuatorManager actuators(LED_PIN, BUZZER_PIN); 
DistanceManager ultrasonic(ULTRA_RX_PIN, ULTRA_TX_PIN);

unsigned long lastUpdate = 0;
const long interval = 1000; 

void onMessage(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  
  JsonDocument doc;
  if (!deserializeJson(doc, msg)) {
    actuators.processCommand(doc);
  }
}

void setup() {
  Serial.begin(115200);
  actuators.begin(); 
  rfid.begin(); 
  ultrasonic.begin();
  
  mqtt.begin();  
  mqtt.setCallback(onMessage);
  mqtt.subscribe(TOPIC_COMMAND); 
}

void loop() {
  mqtt.loop();

  // RFID 
  String uid = rfid.readBadge();
  if (uid != "") {
    String data = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"nfc\": \"" + uid + "\"}";
    mqtt.publish(TOPIC_AUTH, data); 
    actuators.shortBeep();
    delay(2000); 
  }

  // Telemetry 
  unsigned long now = millis();
  if (now - lastUpdate >= interval) {
    lastUpdate = now;
    float dist = ultrasonic.readDistanceCm();
    if (dist != -1.0) {
      String data = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"distance\": " + String(dist) + "}";
      mqtt.publish(TOPIC_TELEMETRY, data); 
    }
  }
}