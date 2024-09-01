//Input Pins
#define STEPPER_HOME_PIN 2
#define MODE_TOGGLE_PIN 42
#define RED_BTN_PIN 20
#define BLUE_BTN_PIN 21
#define ENCODER_A_PIN 19
#define ENCODER_B_PIN 22
#define ENCODER_PRESS_PIN 18

volatile bool encoderTurned = false,
EncoderPreA = false,
EncoderPreB = false,
EncoderNewA = false,
EncoderNewB = false;

bool alreadyTurnedCW = false;
bool alreadyTurnedCCW = false;

volatile bool redBtnPressed = false, 
blueBtnPressed = false, 
encoderPressed = false, 
encoderTurnedCW = false, 
encoderTurnedCCW = false,
stepperIsHome = false,
modeTogglePressed = false;

byte checkedInputs = 0; //Out of 7

void setup() 
{
  EncoderPreA = digitalRead(ENCODER_A_PIN);
  EncoderPreB = digitalRead(ENCODER_B_PIN);
  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(RED_BTN_PIN), redBtnPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BLUE_BTN_PIN), blueBtnPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PRESS_PIN), encoderPress, RISING);
  attachInterrupt(digitalPinToInterrupt(STEPPER_HOME_PIN), stepperHome, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), encoderHasTurned, CHANGE);
  
  
}

void loop() 
{

    if(redBtnPressed)
    {
      redBtnPressed = false;
      Serial.println("Red Button Works");
      checkedInputs++;
    }
    else if(blueBtnPressed)
    {
      blueBtnPressed = false;
      Serial.println("Blue Button Works");
      checkedInputs++;
    }
    else if(encoderPressed)
    {
      encoderPressed = false;
      Serial.println("Encoder-Press Works");
      checkedInputs++;
    }
    else if(stepperIsHome)
    {
      stepperIsHome = false;
      Serial.println("Stepper-Home Works");
      checkedInputs++;
    }
    else if(encoderTurned)
    {
      encoderTurned = false;

      if(encoderTurnedCW && !alreadyTurnedCW)
      {
        alreadyTurnedCW = true;
        Serial.println("Encoder CW Turn Works");
        checkedInputs++;
      }
      else if(encoderTurnedCCW && !alreadyTurnedCCW)
      {
        alreadyTurnedCCW = true;
        Serial.println("Encoder CCW Turn Works");
        checkedInputs++;
      }
    }       
  

  if(!modeTogglePressed && digitalRead(MODE_TOGGLE_PIN))
  {
      modeTogglePressed = true;
      Serial.println("Mode Toggle Works");
      checkedInputs++;
  }
    
  if(checkedInputs == 7)
  {
    Serial.println("All inputs working");
    while(true);
  }
}

void redBtnPress()
{
  redBtnPressed = true;
  detachInterrupt(digitalPinToInterrupt(RED_BTN_PIN));
}

void blueBtnPress()
{
  blueBtnPressed = true;
  detachInterrupt(digitalPinToInterrupt(BLUE_BTN_PIN));
}

void encoderPress()
{
  encoderPressed = true;
  detachInterrupt(digitalPinToInterrupt(ENCODER_PRESS_PIN));
}

void stepperHome()
{
  stepperIsHome = true;
  detachInterrupt(digitalPinToInterrupt(STEPPER_HOME_PIN));
}

void encoderHasTurned()
{
  EncoderNewA = digitalRead(ENCODER_A_PIN);
  EncoderNewB = digitalRead(ENCODER_B_PIN);

  //----------------- Check direction------------------
  if(!EncoderPreA && EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 01 -> 10
  {
    encoderTurnedCW = true;
    encoderTurned = true;
  }
  else if(EncoderPreA && !EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 10 -> 01
  {
    encoderTurnedCW = true;
    encoderTurned = true;
  }
  else if(EncoderPreA && EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 11 -> 01
  {
    encoderTurnedCW = true;
    encoderTurned = true;
  }
  else if(!EncoderPreA && !EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 00 -> 10
  {
    encoderTurnedCW = true;
    encoderTurned = true;
  }
  else if(!EncoderPreA && !EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 00 -> 11
  {
    encoderTurnedCCW = true;
    encoderTurned = true;
  }
  else if(EncoderPreA && EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 11 -> 00
  {
    encoderTurnedCCW = true;
    encoderTurned = true;
  }
  else if(EncoderPreA && !EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 10 -> 00
  {
    encoderTurnedCCW = true;
    encoderTurned = true;
  }
  else if(!EncoderPreA && EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 01 -> 11
  { 
    encoderTurnedCCW = true;   
    encoderTurned = true;
  }

  EncoderPreA = EncoderNewA;
  EncoderPreB = EncoderNewB;
}
