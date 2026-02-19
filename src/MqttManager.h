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

      Serial.println("\nWiFi connecté !");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    }

    // Fonction interne pour se reconnecter si ça coupe
    void reconnect() {
      while (!client.connected()) {
        Serial.print("Connexion MQTT...");
        String clientId = "ESP32Lab-" + WiFi.macAddress();
        
        if (client.connect(clientId.c_str())) {
          Serial.println("Connecté !");
        } else {
          Serial.print("Echec, rc=");
          Serial.print(client.state());
          Serial.println(" (nouvelle tentative dans 5s)");
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

    // Fonction facile pour envoyer un message (IA)
    void publish(const char* topic, String message) {
      client.publish(topic, message.c_str());
    }
    
    String getMacAddress() {
      return WiFi.macAddress();
    }
};

#endif