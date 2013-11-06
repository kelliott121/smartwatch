#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SdFat.h>
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
  tft.fillScreen(BLACK);

  delay(10000);
}

uint8_t val = 0;
void loop()
{
  
  for (byte brightness = 0; brightness < 16; brightness++)
  {
    setBrightness(brightness);
    for (byte height = 0; height <= 128; height+=32)
    {
      tft.fillScreen(BLACK);
      tft.fillRect(0, 0, 128, height, WHITE);
      Serial.print((height * 100)/128, DEC);
      Serial.print("% ");
      Serial.print(brightness, DEC);
      Serial.println("/16");
      delay(10000);
    }
  }
}

void setBrightness(uint8_t val)
{
  tft.writeCommand(SSD1351_CMD_CONTRASTMASTER);
  tft.writeData(val);
}




