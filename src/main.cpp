#include <Arduino.h>
#include <ArduinoJson.h> 
#include "Secrets.h"      
#include "MqttManager.h"
#include "RfidManager.h"
#include "ActuatorManager.h"
#include "DistanceManager.h" 

const int PIN_SDA_SS = 5;
const int PIN_RST = 4;
const int PIN_LED = 25;
const int PIN_BUZZER = 26;

const int PIN_ULTRASON_RX = 16; // Connecté au fil Jaune (TX capteur)
const int PIN_ULTRASON_TX = 17; // Connecté au fil Blanc (RX capteur)

MqttManager monReseauMQTT(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
RfidManager monLecteurRFID(PIN_SDA_SS, PIN_RST);
ActuatorManager mesActionneurs(PIN_LED, PIN_BUZZER); 
DistanceManager monUltrason(PIN_ULTRASON_RX, PIN_ULTRASON_TX);

unsigned long dernierTempsTelemetrie = 0;
const long intervalleTelemetrie = 1000; 

void receptionMessageMQTT(char* topic, byte* payload, unsigned int length) {
  String messageRecu = "";
  for (int i = 0; i < length; i++) {
    messageRecu += (char)payload[i];
  }
  
  JsonDocument doc;
  DeserializationError erreur = deserializeJson(doc, messageRecu);
  
  if (!erreur) {
    mesActionneurs.traiterCommandeJSON(doc);
  } else {
    Serial.println("Erreur: Message MQTT ignoré ");
  }
}

void setup() {
  Serial.begin(9600);

  mesActionneurs.begin(); 
  monLecteurRFID.begin(); 
  monUltrason.begin(); 
  
  monReseauMQTT.begin();  
  monReseauMQTT.setCallback(receptionMessageMQTT); 
  monReseauMQTT.subscribe(TOPIC_COMMAND);          
}

void loop() {
  monReseauMQTT.loop();

  String uidBadge = monLecteurRFID.lireBadge();
  
  if (uidBadge != "") {
    Serial.print("Badge lu : ");
    Serial.println(uidBadge);

    String payload = "{\"mac\": \"" + monReseauMQTT.getMacAddress() + "\", \"nfc\": \"" + uidBadge + "\"}";
    monReseauMQTT.publish(TOPIC_AUTH, payload); 
    
    mesActionneurs.bipCourt();
    
    delay(2000); 
  }

  
  unsigned long tempsActuel = millis();
  
  if (tempsActuel - dernierTempsTelemetrie >= intervalleTelemetrie) {
    dernierTempsTelemetrie = tempsActuel;

    float distance = monUltrason.lireDistanceCm();

    if (distance != -1.0 || true) {
      String payload = "{\"mac\": \"" + monReseauMQTT.getMacAddress() + "\", \"distance\": " + String(distance) + "}";
      
      monReseauMQTT.publish(TOPIC_TELEMETRY, payload); 
      
      Serial.print("Télémétrie envoyée : ");
      Serial.println(payload);
    }
  }
}