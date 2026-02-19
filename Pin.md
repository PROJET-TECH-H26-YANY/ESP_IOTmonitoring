# Câblage ESP32 – RFID, TFT, NeoPixel, Ultrason, Buzzer

## 1. Lecteur RFID RC522 (SPI)

| Pin RFID | Pin ESP32 | Fonction |
|---------|-----------|----------|
| 3.3V | 3V3 | Alimentation (**PAS de 5V**) |
| RST | D22 | Reset |
| GND | GND | Masse |
| MISO | D19 | SPI MISO |
| MOSI | D23 | SPI MOSI (partagé) |
| SCK | D18 | SPI Clock (partagé) |
| SDA (SS) | D5 | Chip Select RFID |

---

## 2. Écran TFT 2.4" (SPI)

| Pin Écran | Pin ESP32 | Fonction |
|----------|-----------|----------|
| VCC | 3V3 | Alimentation |
| GND | GND | Masse |
| CS | D15 | Chip Select écran |
| RESET | D4 | Reset |
| D/C | D2 | Data / Command |
| SDI (MOSI) | D23 | SPI MOSI (partagé) |
| SCK | D18 | SPI Clock (partagé) |

---

## 3. Anneau NeoPixel (12 LEDs)

| Pin LED | Pin ESP32 | Fonction |
|--------|-----------|----------|
| 5V / VCC | VIN | Alimentation 5V (USB) |
| GND | GND | Masse |
| DI (Data) | D27 | Signal de commande |

---

## 4. Capteur Ultrason A0221AU (UART)

| Pin Capteur | Pin ESP32 | Fonction |
|------------|-----------|----------|
| VCC | 3V3 | Alimentation |
| GND | GND | Masse |
| TX (Jaune) | D16 | RX2 |
| RX (Vert) | D17 | TX2 |

---

## 5. Buzzer actif

| Pin Buzzer | Pin ESP32 | Fonction |
|-----------|-----------|----------|
| + | D32 | Commande ON / OFF |
| - | GND | Masse |

---

## Rappels importants de câblage

- **Pins SPI partagées**  
  - D18 (SCK) → RFID + Écran  
  - D23 (MOSI) → RFID + Écran  

- **Masse (GND)**  
  - Toutes les masses doivent être reliées ensemble sur un GND de l’ESP32

- **Alimentation**  
  - RC522 et écran TFT en **3.3V uniquement**
  - NeoPixel alimenté via **VIN (5V USB)**

---

Prêt pour la structure des fichiers du projet.
