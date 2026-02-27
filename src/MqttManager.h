#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttManager {
  private:
    WiFiClient espClient;
    PubSubClient client;
    const char* _ssid;
    const char* _pass;
    const char* _server;
    int _port;
    const char* _topicToSubscribe = ""; 

    void setup_wifi() {
      delay(10);
      Serial.println();
      Serial.print("Connexion au WiFi : ");
      Serial.println(_ssid);

      WiFi.begin(_ssid, _pass);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("\nWiFi connecté ! IP: ");
      Serial.println(WiFi.localIP());
    }

    void reconnect() {
      while (!client.connected()) {
        Serial.print("Connexion MQTT...");
        String clientId = "ESP32Lab-" + WiFi.macAddress();
        
        if (client.connect(clientId.c_str())) {
          Serial.println("Connecté !");
          if (String(_topicToSubscribe) != "") {
            client.subscribe(_topicToSubscribe);
          }
        } else {
          Serial.print(client.state());
          Serial.println(" Echec nouvelle tentative dans 5s");
          delay(5000);
        }
      }
    }

  public:
    MqttManager(const char* ssid, const char* pass, const char* server, int port) 
      : client(espClient) { 
      _ssid = ssid;
      _pass = pass;
      _server = server;
      _port = port;
    }

    void begin() {
      setup_wifi();
      client.setServer(_server, _port);
    }

    void loop() {
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
    }

    void publish(const char* topic, String message) {
      client.publish(topic, message.c_str());
    }
    
    // IA ma aidé à savoir cpmment intégrer mes topics d'abonnement dans la classe MQTT,
    // et surtout à faire en sorte que l'ESP32 puisse se réabonner automatiquement après une reconnexion 
    // Définir quelle fonction appeler quand on reçoit un message
    void setCallback(MQTT_CALLBACK_SIGNATURE) {
      client.setCallback(callback);
    }

    void subscribe(const char* topic) {
      _topicToSubscribe = topic;
      if (client.connected()) {
        client.subscribe(_topicToSubscribe);
      }
    }
    
    String getMacAddress() {
      return WiFi.macAddress();
    }
};

#endif