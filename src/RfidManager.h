#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <SPI.h>
#include <MFRC522.h>

class RfidManager {
  private:
    MFRC522* mfrc522; 
    int _ssPin, _rstPin;

  public:
    RfidManager(int ssPin, int rstPin) {
      _ssPin = ssPin;
      _rstPin = rstPin;
      mfrc522 = new MFRC522(_ssPin, _rstPin);
    }

    void begin() {
      SPI.begin();         
      mfrc522->PCD_Init(); 
      Serial.println("RFID prêt ");
    }

    String lireBadge() {
      if (!mfrc522->PICC_IsNewCardPresent()) return "";
      if (!mfrc522->PICC_ReadCardSerial()) return "";

      // IA afin d'avoir un format plus lisible et compact que les 10 bytes bruts
      String uid = "";
      for (byte i = 0; i < mfrc522->uid.size; i++) {
        if (mfrc522->uid.uidByte[i] < 0x10) uid += "0"; 
        uid += String(mfrc522->uid.uidByte[i], HEX);
      }
      
      uid.toUpperCase(); 
      mfrc522->PICC_HaltA(); 
      return uid;
    }
};

#endif