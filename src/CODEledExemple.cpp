#include <Arduino.h>
#include <FastLED.h>

// --- Configuration Matérielle ---
constexpr uint8_t LED_PIN = 13;        // Broche GPIO connectée à "IN"
constexpr uint8_t NUM_LEDS = 8;       // Nombre de LEDs sur ton anneau
constexpr uint8_t BRIGHTNESS = 50;     // Luminosité max (0-255)
constexpr auto COLOR_ORDER = GRB;      // Ordre des couleurs standard

// --- Variables Globales ---
CRGB leds[NUM_LEDS];

// --- Prototypes ---
void runLoadingAnimation();

void setup() {
    Serial.begin(115200);
    Serial.println("Initialisation du système LED...");

    // Configuration FastLED (Correction : WS2812B passé en argument de modèle)
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
           .setCorrection(TypicalLEDStrip);
           
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true); // Éteint tout au démarrage pour un état propre
}

void loop() {
    // Exécution de l'animation de manière non-bloquante
    runLoadingAnimation();
    
    // Mise à jour physique des LEDs
    FastLED.show();
}

// --- Implémentation des animations ---

/**
 * @brief Animation "chenillard" avec traînée et changement de couleur fluide.
 * Conçue sans delay() pour permettre l'exécution d'autres tâches dans la loop().
 */
void runLoadingAnimation() {
    static uint32_t lastUpdate = 0;
    static uint8_t currentLed = 0;
    constexpr uint32_t INTERVAL_MS = 50; // Vitesse de rotation en ms

    // Vérifie si le délai est écoulé (non-bloquant)
    if (millis() - lastUpdate >= INTERVAL_MS) {
        lastUpdate = millis();

        // 1. Réduit la luminosité de toutes les LEDs existantes (effet de traînée)
        fadeToBlackBy(leds, NUM_LEDS, 40);

        // 2. Calcule une couleur basée sur le temps (effet arc-en-ciel)
        uint8_t hue = (millis() / 20) % 255; 

        // 3. Allume la LED cible avec la couleur calculée
        leds[currentLed] = CHSV(hue, 255, 255);

        // 4. Incrémente et boucle sur le nombre de LEDs
        currentLed = (currentLed + 1) % NUM_LEDS;
    }
}