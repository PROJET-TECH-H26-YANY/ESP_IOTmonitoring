#ifndef CONFIG_H
#define CONFIG_H

// ==========================================
// 1. NETWORK & MQTT SETTINGS
// ==========================================
const char* WIFI_SSID = "TON_WIFI";
const char* WIFI_PASS = "TON_PASS";

const char* MQTT_SERVER = "185.53.209.197"; 
const int MQTT_PORT = 1883;

// ==========================================
// 2. HARDWARE PINS
// ==========================================
// RFID (SPI)
const int RFID_SS_PIN = 5;
const int RFID_RST_PIN = 4;

// Actuators
const int LED_PIN = 25;
const int BUZZER_PIN = 26;

// Ultrasonic Sensor A0221AU (UART on Serial2)
const int ULTRA_RX_PIN = 16; 
const int ULTRA_TX_PIN = 17; 

// ==========================================
// 3. TIMING & TOPICS
// ==========================================
const long TELEMETRY_INTERVAL = 1000; // Send distance every 1000 ms (1 second)

const char* TOPIC_AUTH      = "labo/device/auth";
const char* TOPIC_TELEMETRY = "labo/device/telemetry";
// Note: The command topic is dynamic, so it's generated directly in main.cpp

#endif