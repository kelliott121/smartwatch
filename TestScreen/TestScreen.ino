#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SdFat.h>
#include <SPI.h>

#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8
#define SD_CS 4 

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

// the file itself
SdFile bmpFile;

// information we extract about the bitmap file
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

SdFat sd;

void setup()
{
  Serial.begin(9600);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  
  tft.begin();
  
  if (!sd.begin(SD_CS, SPI_FULL_SPEED)) {
    sd.initErrorHalt();
    Serial.println("failed!");
    return;
  }
  
  Serial.println("Starting");
  
  tft.fillScreen(BLACK);
  bmpDraw("bluetoot.bmp", 0, 0);
  bmpDraw("battery.bmp", 112, 0);
  bmpDraw("facebook.bmp", 0, 16);
  bmpDraw("gmail.bmp", 0, 32);
  bmpDraw("sms.bmp", 0, 48);
  bmpDraw("twitter.bmp", 0, 64);
}

void loop()
{
  //tft.fillScreen(BLACK);
  tft.setCursor(40, 4);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("12:00 PM");
  tft.setCursor(16, 16);
  tft.setTextStart(16, 16);
  tft.setTextEnd(112, 128);
  tft.println("Big Ben:");
  tft.setCursor(16, 24);
  tft.print("BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG BONG");
  
  tft.drawRect(0, 64, 16, 16, RED);
  
  //tft.drawRect(0, 0, 128, 16, RED);
  //tft.drawRect(0, 16, 16, 112, RED);
  //tft.drawRect(16, 16, 112, 128, RED);
  //tft.drawRect(112, 16, 16, 112, RED);
  delay(5000);
  
  bmpDraw("twitter.bmp", 0, 64);
  tft.drawRect(0, 48, 16, 16, RED);
  tft.fillRect(16, 16, 98, 112, BLACK);
  tft.setCursor(16, 16);
  tft.println("Leanne:");
  tft.setCursor(16, 24);
  tft.print("Hey, I'm going to be late.");
  delay(5000);
  
  bmpDraw("sms.bmp", 0, 48);
  tft.drawRect(0, 32, 16, 16, RED);
  tft.fillRect(16, 16, 98, 112, BLACK);
  tft.setCursor(16, 16);
  tft.println("Nigerian Prince:");
  tft.setCursor(16, 24);
  tft.print("You may have already won!");
  delay(5000);
  
  bmpDraw("gmail.bmp", 0, 32);
  tft.drawRect(0, 16, 16, 16, RED);
  tft.fillRect(16, 16, 98, 112, BLACK);
  tft.setCursor(16, 16);
  tft.println("Leanne updated her status: Going to Chipotle with Keith!");
  delay(5000);
  
  bmpDraw("facebook.bmp", 0, 16);
  tft.fillRect(16, 16, 98, 112, BLACK);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint8_t y) {

  SdFile   bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile.open(filename, O_READ)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        for (row=0; row<h; row++) { // For each scanline...
          tft.goTo(x, y+row);

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.curPosition() != pos) { // Need seek?
            bmpFile.seekSet(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          // optimize by setting pins now
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            tft.drawPixel(x+col, y+row, tft.Color565(r,g,b));
            // optimized!
            //tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(SdFile& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(SdFile& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
