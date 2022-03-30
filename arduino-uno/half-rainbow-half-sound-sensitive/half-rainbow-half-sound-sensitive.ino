#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS_RAINBOW    100
#define NUM_LEDS_TOTAL      300
#define NUM_LEDS_SOUND      NUM_LEDS_TOTAL - NUM_LEDS_RAINBOW       
#define BRIGHTNESS 255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS_TOTAL];

#define UPDATES_PER_SECOND 250

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int updateLEDs=1;
typedef struct color Color;
int valR=0,valG=0,valB=0;

void setup() {
  delay( 500 ); // power-up safety delay
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS_TOTAL).setCorrection( TypicalLEDStrip );
    for(int i = 0; i < NUM_LEDS_TOTAL ; i++) {
      leds[i] = CRGB(0,0,0);
  }
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();
  //delay( 1000 );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

int count = 0;
void loop()
{
  if(count % 10 == 0){
  ChangePalettePeriodically();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);
  count = 0;
  }
  if (Serial.available()) {
      char incoming = Serial.read();
      updateLEDs = incoming - '0' + 1;
  }
  valR = ( updateLEDs > 8 || updateLEDs == 4) * 255;
  valG = ( updateLEDs > 5 && updateLEDs < 8 ) * 255;
  valB = ( updateLEDs > 1 || updateLEDs == 8 ) * 255;
  
  for(int i = NUM_LEDS_TOTAL - 1; i >= NUM_LEDS_TOTAL - 1 - updateLEDs; i--) {
    leds[i] = CRGB(valR,valG,valB);  
  }
  
  for(int i = NUM_LEDS_RAINBOW + 1; i <= NUM_LEDS_TOTAL - updateLEDs -2; i++) {
    leds[i] = leds[i + updateLEDs];
  }
  FastLED.show();
  while(Serial.available()) {
    Serial.read();
  }
  count++;
  //FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS_RAINBOW; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 1;
  }
}


void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }

  }
}
