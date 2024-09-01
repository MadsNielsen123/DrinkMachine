/*
 * This test will try to scroll up and down a simple list using the rotary encoder.
 */

//---------- DISPLAY

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

uint8_t headerFontSize = 3;
uint8_t textFontSize = 2;

//---------- ENCODER

#define EncoderA 18 //Encoder's A output
#define EncoderB 46 //Encoder's B output

volatile bool EncoderState; //State of A output
volatile bool EncoderChanged;  //True if encoder position was changed

// ----------- MISC
const unsigned int numberOfDrinks = 5;
String drinkListe[numberOfDrinks] = {"Vodka", "Sodavand", "Juice", "Gin", "Tequilla"}; //Max name size: 15
unsigned int currentDrinkSelected = 1;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() 
{
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);
  attachInterrupt(digitalPinToInterrupt(18), EncoderTurnISR, RISING);
  
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(0);                 
  updateMenu();
   
}

void loop() 
{
    getEncoderValue();
}

void getEncoderValue()
{
  //check if encoder was turned
  if(EncoderChanged)
  {
    EncoderChanged = false;
    
    if(EncoderState != digitalRead(EncoderB) && currentDrinkSelected != numberOfDrinks)
      currentDrinkSelected++;
    else if(EncoderState == digitalRead(EncoderB) && currentDrinkSelected != 1)
      currentDrinkSelected--;

   updateMenu();
   Serial.println(currentDrinkSelected);
  }
}

void updateMenu()
{
  uint8_t tftWidth = tft.width(); //240
  uint16_t tftHeight = tft.height(); //320
  const uint8_t textSpacing = 8*textFontSize+4;
  int16_t cursorHeight;
  tft.fillRect(0, 8*headerFontSize+2, tftWidth, tftHeight-(8*headerFontSize+2), 0);
  tft.fillRect(0, tftHeight/2-2, tftWidth-10, 7*textFontSize+4, 0x12EB);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(textFontSize);
  for(int i= 1; i<=numberOfDrinks; i++)
  {
    //Drink can be between space of 0-320 (each require 16 + 4 between) Selected drink should be at middle height
    if(i < currentDrinkSelected)
    {
      cursorHeight = (tftHeight/2)-textSpacing*(currentDrinkSelected-i);
    }
    else if(i == currentDrinkSelected)
    {
       cursorHeight = (tftHeight/2);
    }
    else
    {
      cursorHeight = (tftHeight/2)+textSpacing*(i-currentDrinkSelected);
    }

    if(cursorHeight > -1 && cursorHeight < 321)
    {
      tft.setCursor(5, cursorHeight);
      tft.print(i);
      tft.print(". ");
      tft.print(drinkListe[i-1]);   
    }
   
  }
  
  
  
  tft.fillRect(0, 0, tftWidth, 8*headerFontSize+1, 0xFFFF);
  tft.setCursor(5, 1); tft.setTextColor(0);  tft.setTextSize(headerFontSize);
  tft.println("Drink Menu");
  tft.drawFastVLine(tftWidth-10, 8*headerFontSize + 5, tftHeight-(8*headerFontSize+10), ILI9341_CYAN);
}

void EncoderTurnISR()
{
  EncoderChanged = true;
  EncoderState = digitalRead(EncoderA);
}
