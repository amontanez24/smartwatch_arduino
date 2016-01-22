/***************************************************
  This is our Bitmap drawing example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "TouchScreen.h"
#include <SoftwareSerial.h>// import the serial library

int BluetoothData; // the data given from Computer
//int pin = 0;
String readString;

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define SWIPE 100

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int oldcolor, currentcolor, startX, startY;
bool fingerDown = false;
int skipFrame = 0;
int a = 0;
String day;
String clk;
String sunrise;
String sunset;
String temp;
String humidity;
String high;
String low;
String feel;
String precip;
int drawn = 0;
String delimiter = "&;";
String  token;
int i;
String stuff[18];
String sub;
int j = 0;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define SD_CS 4

void setup(void) {
  Serial.begin(9600);
  pinMode(0, INPUT);
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
  }
  Serial.println("OK!");
  tft.setRotation(0);
  bmpDraw("orange.bmp", 0, 0);
//works well
      tft.setRotation(1);
      tft.setCursor(30, 20);
      tft.setTextColor(ILI9341_RED);    
      tft.setTextSize(9);
      tft.print("12:30"); 
      tft.setCursor(25, 160);
      tft.setTextColor(ILI9341_WHITE);    
      tft.setTextSize(7);
      tft.print("12"); 
  a= 1;
}

void loop()

{
   // tft.setRotation(3);
    while(Serial.available()){
      delay(3);
      char c = Serial.read();
      readString += c; 
  }
  if(readString.length() > 0){
    Serial.println("got shit");
    
    for(int k = 0; k <18; k++){
      stuff[k] = "";
     }  
      
    //time to parse this...
    j = 0;
    i = 0;

    while(i < readString.length()){
      sub = readString.substring(i,readString.length());
      token = sub.substring(0, sub.indexOf(delimiter));
      i = i + token.length()+ 2;
      stuff[j] =  token;
      j = j + 1;
    }
    for(int q = 0; q <18; q++){
      Serial.println(stuff[q]);
      
    }
  }
     
     sub = "";
     token = "";
     readString = "";
  // Retrieve a point  
  TSPoint p = ts.getPoint();
  
 /*
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);  
 */


  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     if (skipFrame > 5){
      fingerDown = false;
      skipFrame = 0;
     }
     skipFrame += 1;
     return;
  }

  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  /*
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  */

  /*
   * This is the code to sense if swipe right or swipe left
   */
   if(!fingerDown){
    startX = p.x;
    startY = p.y;
    fingerDown = true;
   } else {
     skipFrame = 0;
     if(p.x - startX > SWIPE){
     //tft.println("SWIPE UP");
     //drawn = 0;
     
      fingerDown = false;
     } else if (startX - p.x > SWIPE ){
     //tft.println("SWIPE DOWN");
     //drawn = 0;
      
      fingerDown = false;
     } else if (startY - p.y > SWIPE){
     //tft.println("SWIPE LEFT");
     drawn = 0;
      
      a =a -1;
      fingerDown = false;
     } else if (p.y - startY  > SWIPE){
     //tft.println("SWIPE RIGHT");
     drawn = 0;
     a =a + 1;
      
      fingerDown = false;
     }


     if (a == 0){
      a = 5;
     }

      if (a == 6){
      a = 1;
     }

    Serial.println(a);

  if (a ==1 && drawn == 0){
    drawn = 1;
    tft.setRotation(0);
    bmpDraw("orange.bmp", 0, 0);
      tft.setRotation(1);
      tft.setCursor(30, 20);
      tft.setTextColor(ILI9341_RED);    
      tft.setTextSize(9);
      tft.print("12:30"); 
      tft.setCursor(30, 70);
      tft.setTextColor(ILI9341_WHITE);    
      tft.setTextSize(2);
      tft.print("12"); 
  
  
  
  
  }
  if (a ==2 && drawn == 0){
   tft.setRotation(0);
    bmpDraw("yellow.bmp", 0, 0);
    drawn = 1;
tft.setRotation(3);
    
  }
  if (a ==3&& drawn == 0){
    tft.setRotation(0);
    drawn = 1;
    bmpDraw("purple.bmp", 0, 0);
tft.setRotation(3);
   

  }
  if (a ==4&& drawn == 0){
    drawn = 1;
    //bmpDraw("purple.bmp", 0, 0);
    //tft.print(a);


   

  }
  if (a ==5&& drawn == 0){
   drawn = 1;
  tft.fillScreen(ILI9341_BLACK);
    
//tft.print(a);
  } 

   }
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
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
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
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

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

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
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

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
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}