#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>

#define OLED_RESET 4
Adafruit_SSD1306 oledDisplay(OLED_RESET);

//make functions to display stuff

void printNum(int num){
  oledDisplay.clearDisplay();
  oledDisplay.setCursor(0,0);
  oledDisplay.print(num);
  oledDisplay.display();
}



