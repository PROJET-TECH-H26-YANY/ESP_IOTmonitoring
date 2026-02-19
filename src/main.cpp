#include <Arduino.h>
#include "Secrets.h"      
#include "MqttManager.h"
#include "RfidManager.h" 

MqttManager monReseauMQTT(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);

const int PIN_SDA_SS = 5;
const int PIN_RST = 4;
RfidManager monLecteurRFID(PIN_SDA_SS, PIN_RST);

void setup() {
  Serial.begin(9600);
  
  monReseauMQTT.begin();
  monLecteurRFID.begin();
}

void loop() {
  monReseauMQTT.loop();

  String uidBadge = monLecteurRFID.lireBadge();

  if (uidBadge != "") {
    Serial.print("Badge détecté ! UID : ");
    Serial.println(uidBadge);

    String payload = "{\"mac\": \"" + monReseauMQTT.getMacAddress() + "\", \"nfc\": \"" + uidBadge + "\"}";
    
    monReseauMQTT.publish(TOPIC_AUTH, payload); 
    
    Serial.println("-> Message envoyé au VPS !");

    delay(4000); 
  }
}