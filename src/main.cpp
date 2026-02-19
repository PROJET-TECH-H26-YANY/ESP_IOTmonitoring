#include <Arduino.h>
#include <ArduinoJson.h>
#include "Secrets.h"      
#include "MqttManager.h"
#include "RfidManager.h"
#include "ActuatorManager.h" 

const int PIN_SDA_SS = 5;
const int PIN_RST = 4;
const int PIN_LED = 25;
const int PIN_BUZZER = 26;

MqttManager monReseauMQTT(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
RfidManager monLecteurRFID(PIN_SDA_SS, PIN_RST);
ActuatorManager mesActionneurs(PIN_LED, PIN_BUZZER); 

void receptionMessageMQTT(char* topic, byte* payload, unsigned int length) {
  String messageRecu = "";
  for (int i = 0; i < length; i++) {
    messageRecu += (char)payload[i];
  }
  
  JsonDocument doc;
  DeserializationError erreur = deserializeJson(doc, messageRecu);
  
  if (!erreur) {
    mesActionneurs.traiterCommandeJSON(doc);
  }
}

void setup() {
  Serial.begin(115200);

  mesActionneurs.begin(); 
  monLecteurRFID.begin();
  
  monReseauMQTT.begin(); 
  monReseauMQTT.setCallback(receptionMessageMQTT);
  monReseauMQTT.subscribe(TOPIC_COMMAND); 
}

void loop() {
  monReseauMQTT.loop();

  String uidBadge = monLecteurRFID.lireBadge();

  if (uidBadge != "") {
    String payload = "{\"mac\": \"" + monReseauMQTT.getMacAddress() + "\", \"nfc\": \"" + uidBadge + "\"}";
    monReseauMQTT.publish(TOPIC_AUTH, payload); 
    
    mesActionneurs.bipCourt();
    
    delay(2000); 
  }
}