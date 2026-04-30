# Guide de Déploiement - ESP32

L'objet connecté utilise **PlatformIO** pour la gestion automatique des dépendances. Aucune bibliothèque Arduino n'a besoin d'être installée manuellement.

## Étape 1 : Prérequis Logiciels
1. Installez [Visual Studio Code](https://code.visualstudio.com/).
2. Dans les extensions VS Code, recherchez et installez **PlatformIO IDE**.
3. Clonez le dépôt du projet ESP32 et ouvrez le dossier avec VS Code.

## Étape 2 : Configuration du Réseau et Serveur

1. Ouvrez le fichier `src/Config.h`.
2. Modifiez les identifiants Wi-Fi avec ceux du réseau sur lequel vous testez :
```cpp
const char *WIFI_SSID = "........";
const char *WIFI_PASS = ".........";

const char *MQTT_SERVER = "185.53.209.197";
```
