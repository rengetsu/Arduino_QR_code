#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define LCD_RESET A4

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);

  //TRYING TO DRAW QR CODE

  tft.fillScreen(WHITE);

  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);

  for (int i = 0; i < 6; i++)
  {
    tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
    tft.fillRect(BOXSIZE, BOXSIZE*(i+1), BOXSIZE, BOXSIZE, BLACK);
    tft.fillRect(BOXSIZE*2, BOXSIZE*(i+1), BOXSIZE, BOXSIZE, WHITE);
    tft.fillRect(BOXSIZE*3, BOXSIZE*(i+1), BOXSIZE, BOXSIZE, BLACK);
    tft.fillRect(BOXSIZE*4, BOXSIZE*(i+1), BOXSIZE, BOXSIZE, WHITE);
    tft.fillRect(BOXSIZE*5, BOXSIZE*(i+1), BOXSIZE, BOXSIZE, BLACK);

    i++;

    tft.fillRect(0, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, BLACK);
    tft.fillRect(BOXSIZE, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, WHITE);
    tft.fillRect(BOXSIZE*2, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, BLACK);
    tft.fillRect(BOXSIZE*3, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, WHITE);
    tft.fillRect(BOXSIZE*4, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, BLACK);
    tft.fillRect(BOXSIZE*5, BOXSIZE*(i+2), BOXSIZE, BOXSIZE, WHITE);
  }



 
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = RED;
 
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    /*
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    */
  }
}

