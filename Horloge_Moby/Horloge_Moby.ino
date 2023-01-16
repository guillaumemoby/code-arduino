#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>
#include <DS3231.h>
#include "SoftwareSerial.h"
SoftwareSerial mySerial(6, 7); // RX, TX

// Horloge
#define DS1307_ADDRESS 0x68
byte zero = 0x00; 

#define PIN_NEO_PIXEL  4   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     144  // The number of LEDs (pixels) on NeoPixel
#define button1 12
#define button2 2
#define IR_RECEIVE_PIN 8

decode_results results;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Variables will change:
int lastState = HIGH; // the previous state from the input pin
int currentState;    // the current reading from the input pin
int but2 = 0;
int red = random(0, 255);
int green = random(0, 255);
int blue = random(0, 255);
int red2 = random(0, 255);
int green2 = random(0, 255);
int blue2 = random(0, 255);
int nbrow = random(1, 9) * 16;
int IR = IrReceiver.decodedIRData.command;
int currentPixel = 0;
int rainbowSpeed = 5;
int brightness = 10; // brightness value between 0 and 255
char w;
String v;
bool alarmOn = true;

int ledMatrix[9][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16},
    {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
    {63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48},
    {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    {95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80},
    {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
    {127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112},
    {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143}
};


int IRState = 0;

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
  mySerial.begin(9600);
  Serial.println("Initialize System");
  delay(1000);
  NeoPixel.begin();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  IrReceiver.begin(IR_RECEIVE_PIN);
  Wire.begin();
}


//MAIN LOOP
void loop() {
// INFRA ROUGE/////
 if (IrReceiver.decode()) {
      //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      IrReceiver.resume();
      //Serial.println(IrReceiver.decodedIRData.command);
  }

//BLUETOOTH///////////  
if (mySerial.available()) {
w = mySerial.read();
//Serial.println(w); //PC
delay(10);
if  (w == 'a'){
  colorchange();
  IRState = 22;
} else if  (w == 'z'){
  randomcolor();
  IRState = 22;
} else if  (w == 'e'){
  IRState = 3;
} else if (w == 'r'){
  drawSmiley();
  IRState = 22;
} else if (w == 't'){
  onecolor();
  IRState = 22;
} else if (w == 'y'){
  rainbow();
  IRState = 22;
} else if (w == 'u'){
  waveColor();
  IRState = 22;
} else if (w == 'i'){
  rainbowfade();
  IRState = 8;
} else if (w == 'o'){
  rainbowVert();
  IRState = 22;
} else if (w == 'p'){
  increaseBrightness();
  IRState = 22;
} else if (w == 'q'){
  decreaseBrightness();
  IRState = 22;
} else if (w == 's'){
  drawJapaneseFlag();
  IRState = 22;
} else if (w == 'd'){
  IRState = 12;
} else if (w == 'f'){
  rainbowCycle();
  IRState = 22;
} else if (w == 'g'){
  progressivestrobe();
  IRState = 22;
}
}

if (mySerial.available()) {
v = mySerial.readString();
v.replace(" ", "");
w = v[0];
Serial.println(w);
}

if (alarmOn) {
  allumerLed22h20();
}

readRTC();

//Serial.println(IRState);
  currentState = digitalRead(button2);
  if(lastState == HIGH && currentState == LOW && but2 == 0 ){
    but2 = 1;
  } else  if(lastState == HIGH && currentState == LOW && but2 == 1 ){
    but2 = 0;
  }  
  lastState = currentState;
  IRChoice();

  
     
}// UTIL
void IRChoice(){
  if (IrReceiver.decodedIRData.decodedRawData == 0xBA45FF00)// 1
  {colorchange();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xB946FF00)//2
  {randomcolor();
  clearState();
  } else if (but2) {
    secondclock();
  } else if (!digitalRead(button1)) {
    colorchange();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xB847FF00)//3
  { turnoff();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xBB44FF00)//4
  { drawSmiley();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xBF40FF00)//5
  { onecolor();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xBC43FF00)//6
  { rainbow();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xF807FF00)//7
  { waveColor();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xEA15FF00)//8
  { IRState = 8;
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xF609FF00)//9
  { rainbowVert();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xF20DFF00)//#
  { defilement();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xE916FF00)//*
  { clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xF708FF00)// fleche de gauche
  { drawJapaneseFlag();
    clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xA55AFF00)// fleche de droite
  { waveAnimation();
    clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xE718FF00)// fleche du haut
  { increaseBrightness();
  clearState();
  } else if (IrReceiver.decodedIRData.decodedRawData == 0xAD52FF00)// fleche du bas
  { decreaseBrightness();
  clearState();
  }  else if (IrReceiver.decodedIRData.decodedRawData == 0xE31CFF00)// ok
  { progressivestrobe();
  clearState();
  } else if (IRState == 8)// 
  { rainbowfade();
  } else if (IRState == 12)// 
  { waveGreen();
  } else if (IRState == 3)// 
  { turnoff();
  }
}



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

void onecolor(){
  NeoPixel.clear(); 
  red = random(0, 255);
  green = random(0, 255);
  blue = random(0, 255);    
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(red, green, blue)); 
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(500);
}

void rainbow() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, Wheel(((pixel * 256 / NUM_PIXELS)) & 255));
  }
  NeoPixel.show();
}

// Helper function to create rainbow pattern
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return NeoPixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return NeoPixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return NeoPixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void rainbowCycle() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, Wheel(((pixel + currentPixel) * 256 / NUM_PIXELS) & 255));
  }
  NeoPixel.show();
  currentPixel += rainbowSpeed;
}

void rainbowFlow() {
  NeoPixel.clear();
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, Wheel(((pixel * 256 / NUM_PIXELS) + currentPixel) & 255));
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  currentPixel += rainbowSpeed;
  delay(20);
}


void rainbowWave() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, Wheel(((pixel + currentPixel) * 256 / NUM_PIXELS) & 255));
    NeoPixel.setBrightness(255 * (NUM_PIXELS - abs(NUM_PIXELS/2 - pixel)) / (NUM_PIXELS/2));
  }
  NeoPixel.show();
  currentPixel++;
  if (currentPixel == 256) {
    currentPixel = 0;
  }
}

void increaseBrightness() {
brightness = min(brightness + 20, 255); // increase the brightness by 20, but make sure it doesn't go above 255
NeoPixel.setBrightness(brightness);
NeoPixel.show();
delay(100);
}

void decreaseBrightness() {
brightness = max(brightness - 20, 0); // decrease the brightness by 20, but make sure it doesn't go below 0
NeoPixel.setBrightness(brightness);
NeoPixel.show();
delay(100);
}


void defilement() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      NeoPixel.setPixelColor(i, NeoPixel.Color((i - (j * 2)) & 255, 255, brightness));
    }
    NeoPixel.show();
    delay(25);
  }
}

void rainbowfade() //////////////////////////////////////////////////////
{
  w = mySerial.read();
//Serial.println(w); //PC
delay(10);
int fadeSpeed = 5;
for (int i = 0; i < 256; i++) {
  for (int j = 0; j < NUM_PIXELS; j++) {
    if (IrReceiver.decodedIRData.decodedRawData != 0){
      IRState = 24;
      return;
    }
    if (w == '1' || w == '2' || w == '3' || w == '4' || w == '5' || w == '6' || w == '7' || w == '9'){
      IRState = 25;
      return;
    }
    NeoPixel.setPixelColor(j, Wheel((i+j) & 255));
  }
  NeoPixel.setBrightness(brightness);
  NeoPixel.show();
  delay(fadeSpeed);
  }
}

void waveColor() {
    int colorIndex = 0;
    while(digitalRead(button1) && digitalRead(button2) && !IrReceiver.decode()) { // cette boucle continue tant que les deux boutons ne sont pas appuyés et que aucune commande IR n'est reçue
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 16; j++) {
                NeoPixel.setPixelColor(ledMatrix[i][j], Wheel(colorIndex));
            }
            NeoPixel.show();
            delay(50);
            colorIndex += 5;
        }
    }
}

void waveGreen() {
  int colorIndex = 0;
  int limit = 0;
for (int i = 0; i < 16; i++) {
limit = i;
for (int j = 0; j < 9; j++) {
if (IrReceiver.decodedIRData.decodedRawData != 0){
  IRState = 24;
  return;
}
NeoPixel.setPixelColor(ledMatrix[j][i], Wheel(((i * 256 / NUM_PIXELS) + colorIndex) & 255));
}
NeoPixel.show();
delay(2);
colorIndex++;
}
}

void drawJapaneseFlag() {
for (int i = 0; i < 9; i++) {
for (int j = 0; j < 16; j++) {
if (sqrt(pow(4 - i, 2) + pow(8 - j, 2)) <= 2) { //condition pour dessiner un cercle de rayon 2 centré aux coordonnées (4,8)
NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::Red);
} else {
NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::White);
}
}
}
NeoPixel.setBrightness(brightness);
NeoPixel.show();
}

void colorScroll() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    for (int j = 0; j < NUM_PIXELS; j++) {
      NeoPixel.setPixelColor(j, Wheel((i+j) & 255));
    }
    NeoPixel.setBrightness(brightness);
    NeoPixel.show();
    delay(50);
  }
}

void waveAnimation() {
    int waveSpeed = 150; // vitesse de la vague en ms
    int waveWidth = 2; // largeur de la vague en nombre de pixels
    int currentWavePosition = 0; // position actuelle de la vague
    int startTime = millis(); // temps de départ de l'animation
    while (millis() - startTime < 5000) { // boucle pendant 5 secondes
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 16; j++) {
                if (j >= currentWavePosition && j < currentWavePosition + waveWidth) { // ici j'ai changé i pour j
                    NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::Red);
                } else {
                    NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::White);
                }
            }
        }
        NeoPixel.show();
        delay(waveSpeed);
        currentWavePosition++;
        if (currentWavePosition + waveWidth > 16) {  // ici j'ai changé 9 pour 16
            currentWavePosition = 0;
        }
    }
}


void drawSmiley(){
for (int i = 0; i < 9; i++) {
for (int j = 0; j < 16; j++) {
if ((i == 2 && j == 5) || (i == 2 && j == 10) || (i == 5 && j >= 4 && j <= 11) || (i == 6 && j >= 4 && j <= 11) || (i == 7 && j == 5) || (i == 7 && j == 10)) {
NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::Yellow);
} else {
NeoPixel.setPixelColor(ledMatrix[i][j], CRGB::DarkBlue);
}
}
}
NeoPixel.setBrightness(brightness);
NeoPixel.show();
}

void rainbowVert(){
    int fadeSpeed = 1;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 9; j++) {
            for(int k = 0; k < 16; k++){
                if (IrReceiver.decodedIRData.decodedRawData != 0){
                  IRState = 24;
                  return;
                }
                NeoPixel.setPixelColor(ledMatrix[j][k], Wheel((i+j+k) & 255));
            }
        }
        NeoPixel.setBrightness(brightness);
        NeoPixel.show();
        delay(fadeSpeed);
    }
}

void clearState(){
  IRState = 0;
}
void progressivestrobe() {
  int delayval = 30; // delay for 30ms
  for (int brightness = 0; brightness <= 255; brightness++) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      NeoPixel.setPixelColor(i, NeoPixel.Color(random(0, 255), random(0, 255), random(0, 255)));
      NeoPixel.setBrightness(brightness);
      NeoPixel.show();
    }
    delay(delayval);
  }
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      NeoPixel.setPixelColor(i, NeoPixel.Color(random(0, 255), random(0, 255), random(0, 255)));
      NeoPixel.setBrightness(255);
      NeoPixel.show();
    }
    delay(delayval);
    for (int i = 0; i < NUM_PIXELS; i++) {
      NeoPixel.setPixelColor(i, 0);
      NeoPixel.show();
    }
    delay(delayval);
  }
  for (int i = 0; i < NUM_PIXELS; i++) {
    NeoPixel.setPixelColor(i, 0);
    NeoPixel.show();
  }
}



void allumerLed22h20() {
int targetHour = 22;
int targetMinute = 26;

Hour = Clock.getHour(h12, PM), DEC;
Minute = Clock.getMinute(), DEC;

if (Hour == targetHour && Minute == targetMinute && !alarmOn) {
for (int i = 0; i < NUM_PIXELS; i++) {
NeoPixel.setPixelColor(i, NeoPixel.Color(0, 255, 0));
}
NeoPixel.show();
alarmOn = true;
}
}
