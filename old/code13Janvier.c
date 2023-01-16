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

int setDate = false;

//Objects
DS3231 Clock;


void setup() {
//sanity check delay
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  delay(1000);
  NeoPixel.begin();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
      if(setDate) setDateTime();    
  Wire.begin();    
 }

int brightness = 10; // brightness value between 0 and 255

//MAIN LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  readRTC();

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



void colorchange()
{     NeoPixel.clear();
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
    delay(10000);
}

void turnoff(){
NeoPixel.clear();
NeoPixel.show();
delay(500);
}

void readRTC( ) { /* function readRTC */
   ////Read Real Time Clock
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

void setDateTime() { /* function setDate */
  ////Set Real Time Clock
  if (Serial.available()) {

    //int _start = millis();

    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);

    Clock.setClockMode(false);  // set to 24h

    Clock.setSecond(Second);
    Clock.setMinute(Minute);
    Clock.setHour(Hour);
    Clock.setDate(Date);
    Clock.setMonth(Month);
    Clock.setYear(Year);
    Clock.setDoW(DoW);

  }
}
void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) { /* function GetDateStuff */
   ////Get date data
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSS, with an 'x' at the end.
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];
  byte j = 0;
  while (!GotString) {
      if (Serial.available()) {
          InChar = Serial.read();
          InString[j] = InChar;
          j += 1;
          if (InChar == 'x') {
              GotString = true;
          }
      }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;
  // now Hour
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;
}