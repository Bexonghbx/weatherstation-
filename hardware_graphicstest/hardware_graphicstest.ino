#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"


//IMPORT IMAGE 
#include "uwiweather.h"

#define TFT_DC    17
#define TFT_CS    5
#define TFT_RST   16
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  19

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
int value =0;

void setup(){
  Serial.begin(115200); 
  tft.begin();
  tft.drawCircle(120, 160, 20, ILI9341_GREEN);
  delay(2000);
  tft.drawCircle(120, 160, 50, ILI9341_GREEN);
  delay(2000);
  tft.drawCircle(120, 160, 100, ILI9341_GREEN);
  delay(2000);
  tft.drawRGBBitmap(0,0, uwiweather, 240, 320);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(15, 210);
  tft.print("Temperature (C)");
  tft.setCursor(15, 245);
  tft.print("Heat Index (F)");
  tft.setCursor(15, 280);
  tft.print("Pressure (Pa)");
  tft.setCursor(135, 210);
  tft.print("Humidity (%)");
  tft.setCursor(135, 245);
  tft.print("Moisture (?)");
  tft.setCursor(135, 280);
  tft.print("Alitude (m)");
  tft.fillRoundRect(15, 220, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(15, 255, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(15, 290, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 220, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 255, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 290, 48, 20, 4, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  tft.setCursor(19, 227);
  tft.print("Hi");
  tft.setCursor(19, 262);
  tft.print("Hi");
  tft.setCursor(19, 297);
  tft.print("Hi");
  tft.setCursor(139, 227);
  tft.print("Hi");
  tft.setCursor(139, 262);
  tft.print("Hi");
  tft.setCursor(139, 297);
  tft.print("Hi");
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
 
}
void loop(void) { 
  delay(1000);
}
