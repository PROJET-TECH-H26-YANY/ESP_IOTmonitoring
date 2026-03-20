#include "MqttManager.h"

MqttManager::MqttManager(const char *ssid, const char *password, const char *mqttServer, int mqttPort)
{
    _ssid = ssid;
    _password = password;
    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _client.setClient(_espClient);
}

void MqttManager::setupWiFi()
{
    delay(10);
    Serial.println("\nConnexion au WiFi: " + String(_ssid));
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connecté ! IP: " + WiFi.localIP().toString());
}

String MqttManager::getMacAddress()
{
    return WiFi.macAddress();
}

void MqttManager::reconnect()
{
    while (!_client.connected())
    {
        Serial.print("Connexion MQTT...");

        String clientId = "ESP32-" + getMacAddress();

        if (_client.connect(clientId.c_str()))
        {
            Serial.println("Connecté !");

            if (_topicEcoute != "")
            {
                _client.subscribe(_topicEcoute.c_str());
                Serial.println("Réabonné au topic: " + _topicEcoute);
            }
        }
        else
        {
            Serial.print("Échec, erreur=");
            Serial.print(_client.state());
            Serial.println(" nouvelle tentative dans 5 secondes");
            delay(5000);
        }
    }
}

void MqttManager::begin()
{
    setupWiFi();
    _client.setServer(_mqttServer, _mqttPort);
}

void MqttManager::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    _client.setCallback(callback);
}

void MqttManager::subscribe(String topic)
{
    _topicEcoute = topic;
    _client.subscribe(_topicEcoute.c_str());
}

void MqttManager::publish(const char *topic, String payload)
{
    _client.publish(topic, payload.c_str());
}

void MqttManager::loop()
{
    if (!_client.connected())
    {
        reconnect();
    }
    _client.loop();
}