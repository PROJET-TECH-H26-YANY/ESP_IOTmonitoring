#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

typedef void (*JsonCommandCallback)(JsonDocument& doc);

class MqttManager
{
private:
  WiFiClient _espClient;
  PubSubClient _client;
  const char *_ssid;
  const char *_password;
  const char *_mqttServer;
  int _mqttPort;
  String _topicEcoute;

  JsonCommandCallback _onCommand;

  void setupWiFi();
  void reconnect();

public:
  MqttManager(const char *ssid, const char *password, const char *mqttServer, int mqttPort);

  void begin();
  void loop();
  void publish(const char *topic, String payload);
  void subscribe(String topic);
  void setCallback(MQTT_CALLBACK_SIGNATURE);
  String getMacAddress();
  void setCommandCallback(JsonCommandCallback callback); 
  void handlePayload(byte* payload, unsigned int length);
};

#endif