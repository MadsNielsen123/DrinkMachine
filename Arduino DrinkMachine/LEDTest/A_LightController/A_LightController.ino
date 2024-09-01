#include "RGBdriver.h"
#define CLK 3//pins definitions for the driver        
#define DIO 4
RGBdriver Driver(CLK,DIO);

volatile bool endCurrentTask = false;
char lightCommand = 'a';

void dataAvailable()
{
  endCurrentTask = true;
}

void setup()  
{ 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), dataAvailable, RISING);
}  
void loop()  
{    
  switch(lightCommand)
  {
    case 'a': LEDsOff(); break;
    case 'b': slowCrossFadeLEDs(); break;
    case 'c': fastCrossFadeLEDs(); break;
    case 'd': policeLight(); break;
    case 'e': redLEDs(); break;
    case 'f': greenLEDs(); break;
    case 'g': blueLEDs(); break;
    case 'h': slowBlinkRedLEDs(); break;
    case 'i': slowBlinkGreenLEDs(); break;           
    case 'j': slowBlinkBlueLEDs(); break;
    case 'k': fastBlinkRedLEDs(); break;
    case 'l': fastBlinkGreenLEDs(); break;
    case 'm': fastBlinkBlueLEDs(); break;
    case 'n': slowBlinkMultipleLEDs(); break;
    case 'o': fastBlinkMultipleLEDs(); break;
    case 'p': slowLEDchange(); break;
    case 'q': fastLEDchange(); break;
    case 'r': redPulse(); break;
    case 's': greenPulse(); break;           
    case 't': bluePulse(); break;
                  
    default:
      LEDsOff();
      break;
  }

  while(Serial.available() < 1); //Wait till theres's a command
  endCurrentTask = false;
  lightCommand = Serial.read();
  serialFlush();
}

void serialFlush()
{
  while(Serial.available() > 0) 
  {
    Serial.read();
  }
}  
