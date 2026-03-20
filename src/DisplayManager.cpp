#include "DisplayManager.h"
#include "Config.h"

// Initialisation de l'objet Adafruit
DisplayManager::DisplayManager() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

void DisplayManager::begin() {
  Wire.begin(SCREEN_SDA, SCREEN_SCL);

  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("OLED erreur d'initialisation !");
    return;
  }
  
  _display.clearDisplay();
  _display.setTextColor(WHITE);
  
  _display.setTextSize(2); 
  
  showMessage("System\nOK  !");
}

// Affiche un message sur l'écran pas sorti de ma tete mais de IA, je connaissais pas la librairie Adafruit_SSD1306, 
// mais c'est cool !
void DisplayManager::showMessage(String message) {
  _display.clearDisplay();
  _display.setCursor(0, 15); 
  _display.println(message);
  _display.display(); 
}

void DisplayManager::clear() {
  _display.clearDisplay();
  _display.display();
}