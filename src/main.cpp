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

bool isSessionActive = false;
unsigned long lastTelemetryTime = 0;

unsigned long lastScanTime = 0;
const long scanCooldown = 2000;

void onMessageReceived(char *topic, byte *payload, unsigned int length)
{
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, msg);

  if (!error)
  {
    actuators.processCommand(doc);

    if (doc.containsKey("led"))
    {
      if (doc["led"] == 100)
      {
        isSessionActive = true;
        Serial.println(">>> Session STARTED : Telemetry is ON");
      }
      else if (doc["led"] == 0)
      {
        isSessionActive = false;
        Serial.println(">>> Session ENDED : Telemetry is OFF");
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);
  actuators.begin();
  rfid.begin();
  ultrasonic.begin();
  mqtt.begin();
  mqtt.setCallback(onMessageReceived);

  String listenTopic = "labo/device/" + mqtt.getMacAddress() + "/command";
  mqtt.subscribe(listenTopic); 

  Serial.println("System Ready! Listening on: " + listenTopic);
}

void loop()
{
  mqtt.loop();
  unsigned long currentTime = millis();

    String badgeUid = rfid.readBadge();
    if (badgeUid != "")
    {
      Serial.println("Badge Scanned: " + badgeUid);
      lastScanTime = currentTime;
      String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"nfc\": \"" + badgeUid + "\"}";
      mqtt.publish(TOPIC_AUTH, payload);
      actuators.shortBeep();

      delay(1000);
    }
  

  if (isSessionActive)
  {
    if (currentTime - lastTelemetryTime >= TELEMETRY_INTERVAL)
    {
      lastTelemetryTime = currentTime;

      float distance = ultrasonic.readDistanceCm();

      if (distance != -1.0)
      {
        String payload = "{\"mac\": \"" + mqtt.getMacAddress() + "\", \"distance\": " + String(distance) + "}";
        mqtt.publish(TOPIC_TELEMETRY, payload);
      }
    }
  }
}