# Objet Connecté - IoT Monitoring (ESP32)

Ce projet gère la partie matérielle du système d'assiduité. Il utilise un ESP32 pour lire des badges NFC, détecter une présence, afficher le statut et communiquer avec l'API centrale via MQTT.

## Matériel requis
- 1x Microcontrôleur ESP32
- 1x Lecteur RFID RC522 (SPI)
- 1x Capteur Ultrason A0221AU (UART)
- 1x Écran OLED SSD1306 (I2C)
- 1x LED simple
- 1x Buzzer actif

## Schéma de Câblage (Basé sur Config.h)

Assurez-vous de respecter ce câblage exact pour que le code fonctionne sans modification.

| Composant | Broche Composant | Broche ESP32 | Remarques |
| :--- | :--- | :--- | :--- |
| **RFID RC522** | 3.3V | 3V3 | **Attention : PAS 5V** |
| | GND | GND | |
| | RST | D4 | |
| | SDA (SS) | D5 | |
| | MOSI | D23 | SPI matériel par défaut |
| | MISO | D19 | SPI matériel par défaut |
| | SCK | D18 | SPI matériel par défaut |
| **Écran OLED** | VCC | 3V3 | |
| | GND | GND | |
| | SDA | D21 | |
| | SCL | D22 | |
| **Ultrason** | VCC | 3V3 / 5V | Selon modèle |
| | GND | GND | |
| | TX | D16 | (RX2 ESP32) |
| | RX | D17 | (TX2 ESP32) |
| **Actionneurs**| LED | D25 | Mettre une résistance de 220Ω |
| | Buzzer | D26 | |