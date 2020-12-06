/* 
  Example "Protest scroller" for 8x32 WS2812 "NeoPixel Display
  Assumes you are running a standard Arduino ATMega328 compatible board
*/

#include "FastLED.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
// DATA PIN
// If you want to use a different data pin on your microcontroller CHANGE THIS!
#define DATA_PIN 8

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

// Matrix setup params
/*Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, DATA_PIN,
NEO_MATRIX_TOP    + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE + NEO_MATRIX_ZIGZAG,
NEO_GRB            + NEO_KHZ800);
*/
#define pixelPerChar  4
#define delay_scroll 100

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, DATA_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
// Edit this
// The important stuff, your message and colors

//char* Text[]= {"Hello Friends","Welcome To","EASY","HOMEMADE","PROJECTS.","PLZ","LIKE,","SHARE,","& SUBSCRIBE"};

//char* Text[]= {"Hola  ","Bienvenidos a ","javi-ard-pro ","Proyectos", "personales ", "con arduino", " y DIY ","Saludos!"};

const uint16_t colors[] = {
  matrix.Color(255, 0, 255), matrix.Color(100, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};
#define BRIGHTNESS  10
// End Edit


int maxDisplacement;
int mode = 0;
int numColor = arr_len(colors)-1;

//***Variables y funciones para librería y efectos FASTLED
const long javiPic[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 
//0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

const long javiPic2[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000

  
};

const long smiley[] PROGMEM =
{
0x3300ff, 0x3300ff, 0x3300ff, 0x3300ff, 0x3300ff, 0x3300ff, 0x3300ff, 0x3300ff, 
//0x3300ff, 0x3300ff, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x3300ff, 0x3300ff, 
0x3300ff, 0x3300ff, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x3300ff, 0x3300ff, 
0x3300ff, 0xffff00, 0x000000, 0xffff00, 0xffff00, 0x000000, 0xffff00, 0x3300ff, 
//0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 
0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00,
0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 
//0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 
0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00, 0x000000, 0xffff00, 0xffff00,
0x3300ff, 0xffff00, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0x3300ff, 
//0x3300ff, 0x3300ff, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x3300ff, 0x3300ff
0x3300ff, 0x3300ff, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x3300ff, 0x3300ff
};
#define DATA_PIN    8
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    64
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         10
#define FRAMES_PER_SECOND  120
FASTLED_USING_NAMESPACE
byte elegir_libreria =0;
byte addr=0;

void setup() { 

    //iniciamos librerías
iniciar_neopixel(BRIGHTNESS);

 iniciar_fastled(BRIGHTNESS);
 if(EEPROM.read(addr)==0){
  EEPROM.write(addr, 1);
  elegir_libreria=0;
 }
 else{
  EEPROM.write(addr, 0);
   elegir_libreria=1;
 }
   
  
}


void iniciar_neopixel(byte brightness){
   matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
  matrix.setTextColor(colors[0]);
}
void iniciar_fastled(byte brightness){
   delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(brightness);
}


int y = matrix.height();
int x = matrix.width();
int pass = 0;
int line_pass = 0;


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm,read_display_array_fromflash,read_display_array_fromflash2,read_display_array_fromflash_smiley};
//SimplePatternList gPatterns = {read_display_array_fromflash,read_display_array_fromflash2,read_display_array_fromflash_smiley};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
    //iniciamos fastled
    
   
 if(elegir_libreria == 0)
       fastled_funciones(BRIGHTNESS);
   else
    neopixel_funciones( mode);
//  Serial.print(F("El valor de elegir libreria es "));     
//    Serial.print(elegir_libreria);


}

void neopixel_funciones(int mode)
{
  // if mode greater than numMode reset
  
  
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
 
   
  scroll(delay_scroll);
  
}

void fastled_funciones(byte brightness){
 FastLED.setBrightness(brightness);
gPatterns[gCurrentPatternNumber]();
fastled_display();
EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}
void fastled_display(){
 FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  //delay(1000);
  
}
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// this does the magic of scrolling
void scroll(int delays) {
  char* message[]= {"Hola","Bienvenidos","javi-ard-pro","Proyectos", "personales. ", "con arduino", " y DIY ","Saludos!"};

int numMode = arr_len(message)-1;


  //if(++line_pass > matrix.width()) line_pass = 0;
 
  if(--x < -maxDisplacement) {
    x = matrix.width();
    if(++pass >= numColor) { pass = 0;  };
    matrix.setTextColor(colors[pass]);
     maxDisplacement = strlen(message[mode]) * pixelPerChar + matrix.width();
 
    mode++;
  }
 matrix.print(String(message[mode]));
  matrix.show();
  delay(delays);
    if(mode > numMode)
    mode=0;
}

void read_display_array_fromflash(){
// FastLED.clear();
  for(byte i=0; i<NUM_LEDS;i++){
    leds[i]=pgm_read_dword(&(javiPic[i]));
  }
 // FastLED.show();  
  //delay(1000);
}

void read_display_array_fromflash2(){
// FastLED.clear();
  for(byte i=0; i<NUM_LEDS;i++){
    leds[i]=pgm_read_dword(&(javiPic2[i]));
  }
 // FastLED.show();  
  //delay(1000);
}

void read_display_array_fromflash_smiley(){
// FastLED.clear();
  for(byte i=0; i<NUM_LEDS;i++){
    leds[i]=pgm_read_dword(&(smiley[i]));
  }
 // FastLED.show();  
  //delay(1000);
}
