#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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