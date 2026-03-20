#ifndef CONFIG_H
#define CONFIG_H

const char *WIFI_SSID = "Sk-y";
const char *WIFI_PASS = "Sk1ndy2608";

const char *MQTT_SERVER = "185.53.209.197";
const int MQTT_PORT = 1883;

const int RFID_SS_PIN = 5;
const int RFID_RST_PIN = 4;

const int LED_PIN = 25;
const int BUZZER_PIN = 26;

const int ULTRA_RX_PIN = 16;
const int ULTRA_TX_PIN = 17;

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1; 

const int SCREEN_SDA = 21;
const int SCREEN_SCL = 22;

const long TELEMETRY_INTERVAL = 1000;

const char *TOPIC_AUTH = "labo/device/auth";
const char *TOPIC_TELEMETRY = "labo/device/telemetry";

#endif