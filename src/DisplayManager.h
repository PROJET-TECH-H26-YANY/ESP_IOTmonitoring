#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 

class DisplayManager {
  private:
    Adafruit_SSD1306 _display;

  public:
    DisplayManager();
    void begin();
    
    void showMessage(String message);
    
    void clear();
};

#endif