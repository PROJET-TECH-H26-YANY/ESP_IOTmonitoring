#include "RfidManager.h"

RfidManager::RfidManager(int ssPin, int rstPin)
{
  _ssPin = ssPin;
  _rstPin = rstPin;
  _mfrc522 = new MFRC522(_ssPin, _rstPin);
}

void RfidManager::begin()
{
  SPI.begin();
  _mfrc522->PCD_Init();
  Serial.println("RFID Reader Ready");
}

// Logique de lecture du badge RFID basé sur la bibliothèque MFRC522 et les exemples du net
// IA a transform.
// Source : https://www.youtube.com/watch?v=pJLjFm4Ipro
String RfidManager::readBadge()
{
  if (!_mfrc522->PICC_IsNewCardPresent())
    return "";
  if (!_mfrc522->PICC_ReadCardSerial())
    return "";

  String uid = "";
  for (byte i = 0; i < _mfrc522->uid.size; i++)
  {
    if (_mfrc522->uid.uidByte[i] < 0x10)
      uid += "0";
    uid += String(_mfrc522->uid.uidByte[i], HEX);
  }

  uid.toUpperCase();
  _mfrc522->PICC_HaltA();

  return uid;
}