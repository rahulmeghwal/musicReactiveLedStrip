#include "FastLED.h"

#define NUM_LEDS 300        // How many leds in your strip?
#define COLOR_SHIFT 180000  // Time for colours to shift to a new spectrum (in ms)
CRGB leds[NUM_LEDS];        // Define the array of leds

// Define the digital I/O PINS..
#define DATA_PIN 6          // led data transfer

int updateLEDs=1;
typedef struct color Color;
int valR=0,valG=0,valB=0;
void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    
    for(int i = 0; i < NUM_LEDS ; i++) {
      leds[i] = CRGB(0,0,0);
    }
    FastLED.show();
}

void loop() { 
  if (Serial.available()) {
      char incoming = Serial.read();
      updateLEDs = incoming - '0' + 1;
  }
  valR = ( updateLEDs > 8 || updateLEDs == 4) * 255;
  valG = ( updateLEDs > 5 && updateLEDs < 8 ) * 255;
  valB = ( updateLEDs > 1 || updateLEDs == 8 ) * 255;
  for(int i = 0; i < updateLEDs; i++) {
    leds[i] = CRGB(valR,valG,valB);  
  }
  for(int i = 0; i < updateLEDs; i++) {
    leds[i] = CRGB(valR,valG,valB);  
  }

  for(int i = NUM_LEDS - 1; i >= updateLEDs; i--) {
    leds[i] = leds[i - updateLEDs];
  }
  FastLED.show();
  while(Serial.available()) {
    Serial.read();
  }
  delay(5);
  
}
