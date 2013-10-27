#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SD.h>
#include <SPI.h>

#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// to draw images from the SD card, we will share the hardware SPI interface
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

void setup()
{
  Serial.begin(9600);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  
  tft.begin();
  
  Serial.println("Starting");
  
  tft.fillScreen(BLACK);
}

void loop()
{
  tft.fillScreen(BLACK);
  tft.setCursor(40, 4);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("12:00 AM");
  tft.setCursor(16, 16);
  tft.setTextStart(16, 16);
  tft.setTextEnd(112, 128);
  tft.println("Big Ben:");
  tft.setCursor(16, 24);
  tft.print("BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG");
  tft.drawRect(0, 0, 128, 16, RED);
  tft.drawRect(0, 16, 16, 112, RED);
  //tft.drawRect(16, 16, 112, 128, RED);
  tft.drawRect(112, 16, 16, 112, RED);
  delay(10000);
}
