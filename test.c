#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_NEO_PIXEL  4   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     96  // The number of LEDs (pixels) on NeoPixel
#define button1 12
#define button2 2

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  NeoPixel.begin();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);   
  Serial.begin(9600);  
}

int brightness = 10; // brightness value between 0 and 255

void loop() {
  int red = random(0, 255);
  int green = random(0, 255);
  int blue = random(0, 255);

  int red2 = random(0, 255);
  int green2 = random(0, 255);
  int blue2 = random(0, 255);
  int nbrow = random(1, 6) * 16;

/* 
if (digitalRead(button2)){
    Serial.println("boutton 2 pressed");
  }
*/
  

  Serial.println("-----------");

  if (!digitalRead(button1)) {

    Serial.println("boutton 1111111111111111111111111111111111111111111111111111111");
    NeoPixel.clear();

    for (int pixel = 0; pixel < NUM_PIXELS-nbrow; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
    }
    for (int pixel = NUM_PIXELS-nbrow; pixel < NUM_PIXELS; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(red2, green2, blue2)); 
    }
  
    NeoPixel.setBrightness(brightness);
    NeoPixel.show(); // envoie les couleurs mises à jour aux LEDs
    delay(500); 
  } else if (digitalRead(button2)) {
    Serial.println("boutton 2 deuxieme");
    // Eteint les LED lorsque le bouton 2 est enfoncé
    NeoPixel.clear();
    NeoPixel.show();
    delay(500);
  } else {
    delay(500); 
  }
}
