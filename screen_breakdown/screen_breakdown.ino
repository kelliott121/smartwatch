// You can use any (4 or) 5 pins 
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

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

void setup(void) {
  tft.begin();
  tft.fillScreen(BLACK);

  for (int i = 0; i < 8; i++)
  {
    delay(100);
    tft.drawRect(i*16, 0, 16, 16, RED); 
  }

  //tft.drawRect(0, 0, 128, 16, RED);
  //tft.drawRect(0, 16, 128, 112, RED);
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 4; y++)
    {
      delay(100);
      tft.drawRect(x*32, y*28 + 16, 32, 28, RED);
    }
  }
  tft.setCursor(8,19);
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
}


char ch = 'A';
void loop(void) {
  tft.print(ch++);
  delay(100);
  tft.fillRect(1, 16, 31, 27, BLACK);
  tft.setCursor(8, 19);
}


