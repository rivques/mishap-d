/**************************************************************************
Based on Adafruit's SSD1306 library example code for the 128x64 OLED.
This code isn't directly used, it's a standalone display demo.
**************************************************************************=
*/

#include <SPI.h>
#include <Wire.h>
#include <Math.h>
#include <stdio.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PinChangeInterrupt.h"

//encoder pins & variables
#define CLK 2
#define DT 3
#define SW 4
#define backButtonPin 6
int counter;
int currentState;
int initState;
unsigned long bebounceDelay =3D 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define textSize 1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     10 // Reset pin # (or -1 if sharing Arduino reset
pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for
128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int numMenu =3D 8; //number of items in the menu
int menuItem =3D 1;
int curArrow;
int lastArrow;
int curDecimal;
int lastDecimal;
int pixelLength =3D 5 * textSize;
int encTics2Switch =3D 3;// for the menu, how many times do you want to mov=
e
the enc before it switches spots
float disNum;
int sigFig =3D 3;
bool backButton =3D false;
float displayedNumber =3D 0;
bool pressed;
int curMenuSelection =3D 1;
float magnitude;
float curNum;
bool magChose =3D false;
bool pmag;
bool cmag;
float pastDisNum;

void setup() {
  Serial.begin(9600);
  pinMode(backButtonPin, INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC =3D generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  ;
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  encSetup();

  txtSetup();


  Serial.println("done setup");

}

void loop() {
  display.display();
  unsigned long loopStart =3D millis();
  encoderValue();
  backButton =3D !digitalRead(backButtonPin);
  if (curMenuSelection =3D=3D 1) { //main menu selection screen
    displayedNumber =3D 0;
    mainMenuSetup();
    int menuSelection =3D menuSelect(counter);
    if (buttonPress() =3D=3D true) {
      curMenuSelection =3D menuSelection + 2;
      display.clearDisplay();
      while (buttonPress() =3D=3D true) {}
    }
  }
  else if (curMenuSelection =3D=3D 2) {
    menuItem1();
  }
  else if (curMenuSelection =3D=3D 3) {
    chooseNumber();
    display.setCursor(0, 0);
    display.print("Choose Number:");
    display.setCursor(0, 8);
    display.print(disNum, 5);//display number
    display.setCursor(0, 16);
    display.print("Number Chosen:");
    display.setCursor(0, 24);
    display.print(displayedNumber, 5);//display number
    if (backButton =3D=3D true) {
      curMenuSelection =3D 1;
    }
  }
  Serial.print("Loop took ");
  Serial.println(millis() - loopStart);
}


void txtSetup() {
  display.setTextSize(textSize);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

void mainMenuSetup() {
  //menu Items
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F(" MenuItem#1"));//prints menu items in order vertically
  display.println(F(" MenuItem#2"));
  display.println(F(" MenuItem#3"));
  display.println(F(" MenuItem#4"));
  display.println(F(" MenuItem#5"));
  display.println(F(" MenuItem#6"));
  display.println(F(" MenuItem#7"));
  display.println(F(" MenuItem#8"));
}

void encSetup() {
  pinMode(CLK, INPUT);//setting inputs for the encoder
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  initState =3D digitalRead(CLK);
  attachInterrupt(0, encoderValue, CHANGE);
  attachInterrupt(1, encoderValue, CHANGE);
  attachPCINT(digitalPinToPCINT(SW), buttonPress, CHANGE);
}


bool buttonPress() {
  int buttonVal =3D digitalRead(SW);//turning raw data to int
  //If we detect LOW signal, button is pressed
  if (buttonVal =3D=3D LOW) {
    if (millis() - bebounceDelay > 200) {//debounce
      //Serial.println("Button pressed!");
    }
    long debounceDelay =3D millis();
  }
  return !buttonVal;
}

void encoderValue() {
  /*
    Tics is the amount of times you want the encoder to "click" before it
switches menu items.
    This is used because the encoder doesn get read perfectly so a coushin
is added to increase accuracy of menu selection.

    Items represents the amount of items in the selected menu.
    The more items the longer you want to be able to scroll before the
selection arrow goes back to the top.
  */
  // Read the current state of CLK
  currentState =3D digitalRead(CLK);
  // If last and current state of CLK are different, then we can be sure
that the pulse occurred
  if (currentState !=3D initState  && currentState =3D=3D 1) {
    // Encoder is rotating counterclockwise so we decrement the counter
    if (digitalRead(DT) !=3D currentState) {
      counter ++;
    }
    else {
      // Encoder is rotating clockwise so we increment the counter
      counter --;
    }
    // print the value in the serial monitor window
  }
  // Remember last CLK state for next cycle
  initState =3D currentState;
  if (counter >=3D numMenu * encTics2Switch) {
    counter =3D 2;//reset counter when it hits the "bottom" of the menu
  }
  if (counter <=3D 1) {
    counter =3D (numMenu * encTics2Switch) - 1;
  }
}

int menuSelect(int enc) {
  encTics2Switch =3D 3;//presets, fully customizable when calling
encoderValue();
  numMenu =3D 8;//items in menu

  curArrow =3D 8 * counterToMenu(encTics2Switch, numMenu);//location of arr=
ow

  display.setCursor(1, curArrow);
  display.print(F(">"));
  ;
  if (lastArrow !=3D curArrow) {

    int bottomOfPixel =3D lastArrow + 6;

    for (int i =3D lastArrow; i <=3D bottomOfPixel;  i++) { //turns all pix=
els
in a 5x6 to black, effectivly deleting the arrow
      for (int ii =3D 1; ii <=3D pixelLength; ii++) { //
        display.drawPixel(ii, i, SSD1306_BLACK);//
      }
      ;
    }
    lastArrow =3D curArrow;
  }
  int menuItem =3D curArrow / 8;
  return menuItem;
}

void chooseNumber() {
  display.clearDisplay();
  cmag =3D magChose;

  if (magChose =3D=3D false) {
    numMenu =3D 10;//items in menu
    encTics2Switch =3D 2;
    curDecimal =3D counter / 2;
    magnitude =3D map(counterToMenu(encTics2Switch, numMenu), 1, 10, -5,
5);//maps values from 1-10 to -5 - 5, this will be the exponent that 10 is
to the power to. This decides the magnitude of the number.
    if (magnitude > 0) {
      disNum =3D round(pow(10, magnitude));//rounds number because some mat=
h
is funky and the rounding the computer does makes the number bad. so i
round it to the magnitude in digits.
    }
    else {
      disNum =3D pow(10, magnitude);
    }
  }
  else if (magChose =3D=3D true) {
    disNum =3D counterToMenu(encTics2Switch, numMenu) * pow(10, magnitude);
//encoder value * 10^magnitude returned from last function =3D the printed
value
  }
  if (buttonPress() =3D=3D true) {
    magChose =3D !magChose;
    while (buttonPress() =3D=3D true) {}
  }
  Serial.println(pastDisNum);
  if (pmag =3D=3D 1 && cmag =3D=3D 0) {
    Serial.println("happeneing");
    displayedNumber =3D displayedNumber + pastDisNum;
  }
  pmag =3D cmag;
  pastDisNum =3D disNum;
}


int counterToMenu(int tics, int items) {
  int menu =3D counter / tics;
  return menu;
}

void menuItem1() { //menu item display
  display.clearDisplay();
  display.print("stuf");
}

void menuItem2() { //menu item display

}