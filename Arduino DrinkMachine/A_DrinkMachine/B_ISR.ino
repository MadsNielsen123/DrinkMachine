void EncoderTurn()
{  
  EncoderNewA = digitalRead(ENCODER_A);
  EncoderNewB = digitalRead(ENCODER_B);

  //----------------- Check direction------------------
  if(!EncoderPreA && EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 01 -> 10
  {
    encoderTurned = true;
    if(encoderValue < MaxEncoderValue)
      encoderValue++;      
    else
      encoderTriedToPassMax = true;
  }
  else if(EncoderPreA && !EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 10 -> 01
  {
    encoderTurned = true;
    if(encoderValue < MaxEncoderValue)
      encoderValue++;      
    else
      encoderTriedToPassMax = true;
  }
  else if(EncoderPreA && EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 11 -> 01
  {
    encoderTurned = true;
    if(encoderValue < MaxEncoderValue)
      encoderValue++;      
    else
      encoderTriedToPassMax = true;
  }
  else if(!EncoderPreA && !EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 00 -> 10
  {
    encoderTurned = true;
    if(encoderValue < MaxEncoderValue)
      encoderValue++;      
    else
      encoderTriedToPassMax = true;
  }
  else if(!EncoderPreA && !EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 00 -> 11
  {
    encoderTurned = true;
    if(encoderValue > MinEncoderValue)
      encoderValue--; 
    else
      encoderTriedToPassMin = true;   
  }
  else if(EncoderPreA && EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 11 -> 00
  {
    encoderTurned = true;
    if(encoderValue > MinEncoderValue)
      encoderValue--; 
    else
      encoderTriedToPassMin = true;   
  }
  else if(EncoderPreA && !EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 10 -> 00
  {
    encoderTurned = true;
    if(encoderValue > MinEncoderValue)
      encoderValue--; 
    else
      encoderTriedToPassMin = true;  
  }
  else if(!EncoderPreA && EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 01 -> 11
  {
    encoderTurned = true;
    if(encoderValue > MinEncoderValue)
      encoderValue--; 
    else
      encoderTriedToPassMin = true;   
  }

  EncoderPreA = EncoderNewA;
  EncoderPreB = EncoderNewB;

  // ------------ Activate Encoder Delay ------------
  bitClear(EIMSK, INT2); //Disable Encoder 
  TCCR4B |= B00001101; //Enable Counter - CTC mode with 1024 prescaler
}

void EncoderPress()
{
  if(lastButtonPressTime + buttonDelay > millis())
    return;    
  lastButtonPressTime = millis();

  encoderPressed = true;
  switch (currentMenu)
  {
    case PUMP_MENU:
      if(currentPumpSelected <= numberOfPumps) //A Pump Selected
      {
        if(calSizeActive)
          currentMenu = CALIBRATION_MENU;
        else if(drinksLoaded[currentPumpSelected-1] != "")
          currentMenu = POUR_MENU;
      }
      else if(currentPumpSelected > numberOfPumps && currentPumpSelected <= numberOfPumps+numberOfDrinkSizes) //A size Selected
      {
        if(calSizeActive)
          currentMenu = PUMP_MENU_SIZE_SELECTION;
        else
          updateQuickSizeSelected = true;
      }
      else if(adminMode)
      {
        calSizeStart = true;
      }
      break;

    case PUMP_MENU_SIZE_SELECTION:
    currentMenu = PUMP_MENU;
    break;
    
    default:
      break;
  }
}

void RedButtonPress()
{
  if(lastButtonPressTime + buttonDelay > millis())
    return;    
  lastButtonPressTime = millis();

  tone(BUZZ_PIN, 1000, 50);
  switch (currentMenu)
  {
    case PUMP_MENU:
      if(currentPumpSelected <= numberOfPumps && adminMode) //A Pump Selected
        currentMenu = LOADED_DRINK_MENU;
      else
        runNextLight = true;
      break;

    case LOADED_DRINK_MENU:
      changeSearchModeFlag = true;
      break;
      
    case MIX_MENU:
    drinkAccepted = true;
    break;

    default:
      runNextLight = true;
      break;
  }
}

void BlueButtonPress()
{
  if(lastButtonPressTime + buttonDelay > millis())
    return;    
  lastButtonPressTime = millis();

  tone(BUZZ_PIN, 1000, 50);
  switch (currentMenu)
  {
    case PUMP_MENU:
      currentMenu = DRINK_MENU;   
      break;

    case DRINK_MENU:
      currentMenu = PUMP_MENU;
      break;

    case LOADED_DRINK_MENU:
      currentMenu = PUMP_MENU;
      break;

    case POUR_MENU:
      currentMenu = PUMP_MENU;
      break;

    case STRENGTH_MENU:
      returnFromDrinkLaucher = true;
      break;
    
    case SIZE_MENU:
      returnFromDrinkLaucher = true;
      break;

    case MIX_MENU:
      returnFromDrinkLaucher = true;
      break;

    default:
      break;
  }
}

void StepperNowHome()
{
  stepperIsHome = true;
}

ISR (TIMER4_COMPA_vect) //Enables encoder again after x secconds
{
  
  TCCR4B |= B00000000; //disable counter
  TCNT4 = 0; //clear counter
  bitSet(EIFR, INTF2); //Clear INTF2 (External Interrupt Flag 2). Why bitset and not bitclear? - Look datasheet page 112
  bitSet(EIMSK, INT2);  //Énable Encoder (external interrupt) 
}
