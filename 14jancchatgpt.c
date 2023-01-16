#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>

// Horloge
#define DS1307_ADDRESS 0x68
byte zero = 0x00; 

#define PIN_NEO_PIXEL  4   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     96  // The number of LEDs (pixels) on NeoPixel
#define button1 12
#define button2 2

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Variables will change:
int lastState = HIGH; // the previous state from the input pin
int currentState;    // the current reading from the input pin
int onoff = 0;
int red = random(0, 255);
int green = random(0, 255);
int blue = random(0, 255);
int red2 = random(0, 255);
int green2 = random(0, 255);
int blue2 = random(0, 255);
int nbrow = random(1, 6) * 16;

//Variables horloge
byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century   = false;
bool h12 ;
bool PM ;

//Objects
DS3231 Clock;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialize System");
  delay(1000);
  NeoPixel.begin();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  Wire.begin();
}

int brightness = 10; // brightness value between 0 and 255

//MAIN LOOP
void loop() {
  currentState = digitalRead(button2);
  if(lastState == HIGH && currentState == LOW && onoff == 0 ){
    onoff = 1;
  } else  if(lastState == HIGH && currentState == LOW && onoff == 1 ){
    onoff = 0;
  }  
  lastState = currentState;

  if (onoff) {
    secondclock();
  } else if (!digitalRead(button1)) {
    colorchange();
  } else {
    keepcolor();
  }
}

// UTIL

void secondclock () {
  NeoPixel.clear();
  red = 255;
  green = 0;
  blue = 0;  
  red2 = 255;
  green2 = 255;
  blue2 = 255;
  for (int pixel = 0; pixel < Clock.getSecond(); pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
  }
  for (int pixel = Clock.getSecond(); pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red2, green2, blue2)); 
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(1000); 
}

void keepcolor() {
  NeoPixel.clear();
  for (int pixel = 0; pixel < NUM_PIXELS-nbrow; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
  }
  for (int pixel = NUM_PIXELS-nbrow; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red2, green2, blue2)); 
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(100); 
}

void colorchange() {
  NeoPixel.clear();
  red = random(0, 255);
  green = random(0, 255);
  blue = random(0, 255);
  red2 = random(0, 255);
  green2 = random(0, 255);
  blue2 = random(0, 255);
  nbrow = random(1, 6) * 16;
  for (int pixel = 0; pixel < NUM_PIXELS-nbrow; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
  }
  for (int pixel = NUM_PIXELS-nbrow; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red2, green2, blue2)); 
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(500);
}

void uplight() {
  brightness = brightness + 20;
  if (brightness > 120) {
    brightness = 20;
  }
  delay(500);
}

void randomcolor() {
  NeoPixel.clear();     
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    red = random(0, 255);
    green = random(0, 255);
    blue = random(0, 255);
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(500);
}

void turnoff() {
  NeoPixel.clear();
  NeoPixel.show();
  delay(500);
}

void readRTC() { 
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
  delay(1000);
}
