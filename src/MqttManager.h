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
        // ID Unique basé sur la MAC address
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
    // Constructeur : on reçoit les infos de connexion
    MqttManager(const char* ssid, const char* pass, const char* server, int port) 
      : client(espClient) { // Initialise le client MQTT avec le client WiFi
      _ssid = ssid;
      _pass = pass;
      _server = server;
      _port = port;
    }

    // Démarrage : lance le WiFi et configure le serveur MQTT
    void begin() {
      setup_wifi();
      client.setServer(_server, _port);
    }

    // La boucle à appeler tout le temps : gère la survie de la connexion
    void loop() {
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
    }

    // Fonction facile pour envoyer un message
    void publish(const char* topic, String message) {
      client.publish(topic, message.c_str());
    }
    
    // Pour récupérer l'adresse MAC (utile pour le JSON)
    String getMacAddress() {
      return WiFi.macAddress();
    }
};

#endif