#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 
 
int br=115200;
int dt=50;
int xPos=3;
int yPos=2;
int deltaX=1;
int deltaY=1;
 
int i;
int j;

int X_START=4;
int X_END=6;

int leftbut=11;
int rightbut=10;
int leftbutval;
int rightbutval;

int itt=0;
#define NOTE_B6  1976
#define NOTE_C7  2093
 
byte frame[8][12];

int score =0;
int buzz = 12;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(br);
  matrix.begin();
  pinMode(leftbut,INPUT_PULLUP);
  pinMode(rightbut,INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  randomSeed(analogRead(A0));
  u8g2.begin();
  u8g2.setBusClock(400000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso58_tn);
  u8g2.setCursor((128 - u8g2.getStrWidth(String(score).c_str())) / 2, 61); // Centers X dynamically, fixes Y near bottom
  
  u8g2.print(score); 
  u8g2.sendBuffer();
}
void loop() {
  leftbutval=digitalRead(leftbut);
  rightbutval=digitalRead(rightbut);
  if (leftbutval==0 && X_START>0){
    X_START--;
    X_END--;
  }
  if (rightbutval==0 && X_END<11){
    X_START++;
    X_END++;
  }
  // Clear frame
  for (i = 0; i < 12; i++) {
    for (j = 0; j < 8; j++) {
      frame[j][i] = 0;
    }
  }

  // Draw bouncing pixel
  frame[yPos][xPos] = 1;
  

  // Draw paddle using X_START & X_END
  for (int x = X_START; x <= X_END; x++) {
    frame[7][x] = 1;
  }

  matrix.renderBitmap(frame, 8, 12);

// left & right walls
if (itt%2==0){

if (xPos == 0 || xPos == 11) {
  deltaX = -deltaX;
  playSound(NOTE_B6, 30);
}

// top wall only
if (yPos == 0) {
  deltaY = -deltaY;
  playSound(NOTE_B6, 30);
}

// paddle row
if (yPos == 7 && deltaY > 0) {

  // hit
  if (xPos >= X_START && xPos <= X_END) {
    deltaY = -deltaY;
    playSound(NOTE_C7, 30);
    score++;
    Serial.println(score);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso58_tn);
    u8g2.setCursor((128 - u8g2.getStrWidth(String(score).c_str())) / 2, 61); // Centers X dynamically, fixes Y near bottom
    u8g2.print(score);
    u8g2.sendBuffer();	
  }

  // miss
  else {
    tone(buzz, 300, 200); // Medium-low pitch
    delay(250);           // Wait for tone + tiny gap
    tone(buzz, 200, 200); // Lower pitch
    delay(250);
    tone(buzz, 100, 500);
    delay(1000);
    xPos = random(1, 11);
    yPos = 0;
    deltaX = 1;
    deltaY = 1;
    X_START=4;
    X_END=6;
   
    Serial.print("Game over. Score: ");
    Serial.println(score);
    score=0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso58_tn);
    u8g2.setCursor((128 - u8g2.getStrWidth(String(score).c_str())) / 2, 61); // Centers X dynamically, fixes Y near bottom
    
    u8g2.print(score);
    u8g2.sendBuffer();
  }
}

  xPos += deltaX;
  yPos += deltaY;
}

  delay(dt);
  itt++;
}

void playSound(int frequency, int duration) {
  tone(buzz, frequency, duration);          // Stop tone
}