#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>//https://github.com/NorthernWidget/DS3231
#ifdef __AVR__
#endif

// Horloge
#define DS1307_ADDRESS 0x68
byte zero = 0x00; // hack fix

// Config 
int setDate = false;

//Objects
DS3231 Clock;


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

// Pour l'horloge
int minutetest = 0;
int hourTest = 0;
char Time[] = "  :  :  ";
char Calendar[] = "  /  /20  ";
byte i, second, minute, hour, day, date, month, year, temperature_lsb;


void setup() {
//sanity check delay
  delay(1000);
  NeoPixel.begin();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  if(setDate) setDateTime();    
  Serial.begin(9600);
}



int brightness = 10; // brightness value between 0 and 255

//MAIN LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
   Serial.println(brightness);

   //Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read());         //0-6 -> sunday – Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());


//Pour le bouton On/Off
  currentState = digitalRead(button2);
  if(lastState == HIGH && currentState == LOW && onoff == 0 ){
    onoff = 1;
  } else  if(lastState == HIGH && currentState == LOW && onoff == 1 ){
    onoff = 0;
  }  
  // save the the last state
  lastState = currentState;


if (onoff) {
  randomcolor();
} else if (!digitalRead(button1)) {
    colorchange();
  } else {
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
  }
}

// ------
// UTIL
// ------

// Init date
void setDateTime()
{
  byte second = 00;   //0-59
  byte minute = 50;   //0-59
  byte hour = 12;     //0-23
  byte weekDay = 1;  //1-7
  byte monthDay= 7; //1-31
  byte month = 11;     //1-12
  byte year = 22;     //0-99
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(zero); //start
  Wire.endTransmission();
}

void colorchange()
{
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
    NeoPixel.show(); // envoie les couleurs mises à jour aux LEDs
    delay(500);
}

void uplight()
{
  brightness = brightness + 20;
    if (brightness > 120)
    {
      brightness = 20;
    }
    delay(500);
}

void randomcolor(){
  NeoPixel.clear();     
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      red = random(0, 255);
      green = random(0, 255);
      blue = random(0, 255);
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
    }
    NeoPixel.setBrightness(brightness);
    NeoPixel.show(); // envoie les couleurs mises à jour aux LEDs
    delay(2000);
}

void turnoff(){
NeoPixel.clear();
NeoPixel.show();
delay(500);
}


byte decToBcd(byte val)
{
  // Convert normal decimal numbers to binary coded decimal
  return ((val / 10 * 16) + (val % 10));
}

byte bcdToDec(byte val)
{
  // Convert binary coded decimal to normal decimal numbers
  return ((val / 16 * 10) + (val % 16));
}

