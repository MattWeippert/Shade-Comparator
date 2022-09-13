#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2)

// TCS3200 pins wiring to Arduino Nano
#define s0 4
#define s1 5
#define s2 6
#define s3 7
#define outPin 8

// Variables
;int red;
int grn;
int blu;
int y;
int u;
int v;
int color;
int count = 0;
long startTiming = 0;
long elapsedTime = 0;

// Declares function that interprets the YUV values as shades of glue from 1 to 16.
// These values were obtained experimentally, that is I tested each of the Garner color standards using this
// program minus the 'getGlueShade' function in order to establish a comfortable range of 'Y' values that were valid for each
// color shade. To those who just use this code without your own testing, you might get different results. Otherwise, you can
// repurpose this code to detect your own shade range.
void getGlueShade() {  
  rgbToYUV();
  if (y < 20)
  color = 1;
  else if (y <= 23)
  color = 2;
  else if (23 < y <= 26)
  color = 3;
  else if (26 < y <= 29)
  color = 4;
  else if (29 < y <= 32)
  color = 5;
  else if (32 < y <= 35) 
  color = 6;
  else if (35 < y <= 38) 
  color = 7;
  else if (38 < y <= 42) 
  color = 8;
  else if (42 < y <= 45) 
  color = 9;
  else if (45 < y <= 48)
  color = 10;
}

//Converts RGB colors to YUV, which should remove the impact different thicknesses of gluestick.
//Since all we care about is the relative darkness of the glue itself, and color doesn't vary much on an RGB scle for glue products,
//converting to YUV makes for a simpler shading analysis.
void rgbToYUV() {
  readRGB();
  y = (0.299*red) + (0.587*grn) + (0.114*blu);
  u = (blu - y) * 0.565;
  v = (red - y) * 0.713;
}

//Declaration of the color-reading function.
void readRGB() {
  red = 0;
  grn = 0;
  blu = 0;
  int n = 10;
  for (int i = 0; i < n; ++i)
  {
    //read red component
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);
  
   //read green component
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    grn = grn + pulseIn(outPin, LOW);
    
   //let's read blue component
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blu = blu + pulseIn(outPin, LOW);
  }
  red = red/n;
  grn = grn/n;
  blu = blu/n;
}

// Declaration of the functions and parameters which tell the microcontroller how to display information
// obtained from color detector on the display module.
// Note: The information printed on the top row of the LCD screen is meant for testing and debugging,
// if this is too busy or conjested you may remove it.
void showDataLCD()
{
  lcd.setCursor (0,0);
  lcd.print("Y");
  lcd.setCursor (1,0);
  lcd.print("    ");
  lcd.setCursor (1,0);
  lcd.print(y);
  lcd.setCursor (5,0);
  lcd.print(" U");
  lcd.setCursor (7,0);
  lcd.print("    ");
  lcd.setCursor (7,0);
  lcd.print(u);
  lcd.setCursor (12,0);
  lcd.print("V");
  lcd.setCursor (13,0);
  lcd.print("    ");
  lcd.setCursor (13,0);
  lcd.print(v);
  
  lcd.setCursor (0,1);
  lcd.print("Shade: ");
  lcd.setCursor (7,1); 
  lcd.print("        ");
  lcd.setCursor (7,1);  
  lcd.print(color);
  }
  
void setup() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);

  digitalWrite(s0, LOW); //In order to get a larger range of 'Y' values, the light must be kept low in order to maximize the detectable
  digitalWrite(s1, HIGH); //difference between shades of glue.

  Serial.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();
  showDataLCD();
}

void loop() {
  getGlueShade();
  elapsedTime = millis()-startTiming; 
  if (elapsedTime > 1000) 
  {
    showDataLCD();
    startTiming = millis();
  }
}
