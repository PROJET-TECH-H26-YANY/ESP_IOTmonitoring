#include <Arduino.h>
#include <ArduinoJson.h>
#include "Config.h"        
#include "MqttManager.h"
#include "RfidManager.h"
#include "ActuatorManager.h"
#include "DistanceManager.h"


MqttManager mqtt(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
RfidManager rfid(RFID_SS_PIN, RFID_RST_PIN);
ActuatorManager actuators(LED_PIN, BUZZER_PIN); 
DistanceManager ultrasonic(ULTRA_RX_PIN, ULTRA_TX_PIN);

// 2. State Variables (Machine à états)
bool isSessionActive = false;           // Flag to know if student is logged in
unsigned long lastTelemetryTime = 0;    // Timer for distance sending

// ==========================================
// MQTT CALLBACK (When API sends a command)
// ==========================================
void onMessageReceived(char* topic, byte* payload, unsigned int length) {
  // Convert payload to String
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, msg);
  
  if (!error) {
    // 1. Send the command to LED & Buzzer
    actuators.processCommand(doc);
    
    // 2. Update Session State based on LED command
    if (doc.containsKey("led")) {
      if (doc["led"] == 100) {
        isSessionActive = true;
        Serial.println(">>> Session STARTED : Telemetry is ON");
      } 
      else if (doc["led"] == 0) {
        isSessionActive = false;
        Serial.println(">>> Session ENDED : Telemetry is OFF");
      }
    }
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);

  // Start Hardware
  actuators.begin(); 
  rfid.begin(); 
  ultrasonic.begin();
  
  // Start Network & MQTT
  mqtt.begin();  
  mqtt.setCallback(onMessageReceived);

  // Generate dynamic topic for THIS specific ESP32 using its MAC address
  String listenTopic = "labo/device/" + mqtt.getMacAddress() + "/command";
  mqtt.subscribe(listenTopic.c_str()); 
  
  Serial.println("System Ready! Listening on: " + listenTopic);
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  // Keep MQTT connection alive
  mqtt.loop();

  // --- TASK 1: READ RFID BADGE (Always running) ---
  String badgeUid = rfid.readBadge();
  
  if (badgeUid != "") {
    Serial.println("Badge Scanned: " + badgeUid);
    
    // Create and send JSON: {"mac": "...", "nfc": "..."}
    String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"nfc\": \"" + badgeUid + "\"}";
    mqtt.publish(TOPIC_AUTH, payload); 
    
    actuators.shortBeep(); // Local confirmation beep
    delay(2000); // Wait 2 seconds to avoid multiple scans
  }

  // --- TASK 2: SEND TELEMETRY (Only if session is active) ---
  if (isSessionActive) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastTelemetryTime >= TELEMETRY_INTERVAL) {
      lastTelemetryTime = currentTime;

      // Read distance from UART sensor
      float distance = ultrasonic.readDistanceCm();
      
      // If sensor read is valid
      if (distance != -1.0) {
        String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"distance\": " + String(distance) + "}";
        mqtt.publish(TOPIC_TELEMETRY, payload); 
      }
    }
  }
}