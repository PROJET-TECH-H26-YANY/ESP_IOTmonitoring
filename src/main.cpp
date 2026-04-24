#include <Arduino.h>
#include <ArduinoJson.h>

#include "MqttManager.h"
#include "BadgeManager.h"
#include "TelemetryManager.h"
#include "DisplayManager.h"

const char* WIFI_SSID = "TON_WIFI";
const char* WIFI_PASS = "TON_MOT_DE_PASSE";
const char* MQTT_SERVER = "185.53.209.197";
const int   MQTT_PORT = 1883;

MqttManager mqtt(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
DisplayManager oled;

bool isSessionActive = false;


void processServerCommand(JsonDocument& doc) {
  
  if (doc.containsKey("led")) {
    if (doc["led"] == 100) {
      isSessionActive = true;
      Serial.println(">>> Session STARTED : Telemetry is ON");
    } else if (doc["led"] == 0) {
      isSessionActive = false;
      Serial.println(">>> Session ENDED : Telemetry is OFF");
    }
  }

  if (doc.containsKey("oled")) {
    String texteEcran = doc["oled"].as<String>();
    oled.showMessage(texteEcran);
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  mqtt.handlePayload(payload, length); 
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Démarrage Smart Lab IoT ---");

  oled.begin();
  mqtt.setCallback(mqttCallback);
  mqtt.setCommandCallback(processServerCommand); // On lie notre logique métier au manager
  mqtt.begin(); // Se connecte au WiFi et configure le serveur MQTT

  String listenTopic = "labo/device/" + mqtt.getMacAddress() + "/command";
  mqtt.subscribe(listenTopic);
}


void loop() {
  mqtt.loop();
  handleBadge();
  handleTelemetry();
}