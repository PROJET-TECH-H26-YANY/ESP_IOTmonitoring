#include <Arduino.h>
#include "Secrets.h"      
#include "MqttManager.h"

MqttManager monReseauMQTT(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);

unsigned long dernierTempsEnvoi = 0;
const long delaiEntreEnvois = 5000;

void setup() {
  Serial.begin(9600);
  monReseauMQTT.begin();
}

void loop() {
  monReseauMQTT.loop();

  // On vérifie si 5 secondes se sont écoulées depuis le dernier envoi
  unsigned long tempsActuel = millis();
  if (tempsActuel - dernierTempsEnvoi >= delaiEntreEnvois) {
    dernierTempsEnvoi = tempsActuel;

    // Création d'un message simple
    String messageTest = "{\"message\": \"Bonjour depuis l'ESP32 !\"}";
    
    // On publie le message sur le topic de test
    monReseauMQTT.publish("test", messageTest);
    
    Serial.println("Message de test envoyé au VPS !");
  }
}