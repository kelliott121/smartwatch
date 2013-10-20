#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// If we are using the hardware SPI interface, these are the pins (for future ref)
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

void setup(void) {
  Serial.begin(115200);
  Serial.print("hello!");
  tft.begin();

  tft.fillScreen(BLACK);

  Serial.println("init");
  //Larger sizes could result in RAM overflows
  //Grow the square
  for (int side = 1; side <= 32; side++){
    uint16_t pixels[side * side];
    for (int h = 0; h <= side; h++){
      for (int w = 0; w <= side; w++){
        pixels[w + (h * side)] = RED;
      }
    }
    tft.fillRect(0, 0, side, side, BLACK);
    tft.writePixels(0, 0, side, side, pixels); 
    delay(100);
  }

  Serial.println("done");
  delay(100);

  tft.fillScreen(BLACK);
  char x = 11;
  char y = 0;
  char side = 16;
  char speedX = 4;
  char speedY = 4;
  while (1){
    //Serial.print("X: ");
    //Serial.println(x, DEC);
    //Serial.print("Y: ");
    //Serial.println(y, DEC);
    //Move the square
    tft.fillRect(x, y, side, side, BLACK);
    x += speedX;
    y += speedY;
    tft.fillRect(x, y, side, side, RED); 
    //delay(10);
    
    if ((x + side >= 128) || (x <= 0)){
      speedX = -speedX;
    }
    
    if ((y + side >= 128) || (y <= 0)){
      speedY = -speedY;
    }
  }
}


void loop() {
}




