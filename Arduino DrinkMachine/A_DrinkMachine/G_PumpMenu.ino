void pumpMenu()
{
  adminMode = digitalRead(MODE_BTN);
  currentPumpSelected = 1;
  calSizeActive = false;
  pumpMenuSetup();
  while (currentMenu == PUMP_MENU || currentMenu == PUMP_MENU_SIZE_SELECTION)
  { 
       
    if (encoderTurned)
      updatePumpSelected();

    if(updateQuickSizeSelected)
      updateQuickSize();
    
    if(runNextLight)
        nextLight();

    if(calSizeStart)
      toggleCalSize();

    if(currentMenu == PUMP_MENU_SIZE_SELECTION)
      changeSize();      
  
  }
  adminMode = false;
  encoderPressed = false;
}

void pumpMenuSetup()
{
  tft.fillScreen(0); //Clear screen

  //TOP
  tft.drawFastHLine(0, 30, tftWidth, 0xFFFF);
  tft.drawFastVLine(96, 0, 30, 0xFFFF);
  tft.drawFastVLine(192, 0, 30, 0xFFFF);
  tft.fillRect(97, 0, 95, 30, 0x551F);
  tft.setTextSize(2);
  tft.setCursor(10, 7);
  tft.setTextColor(0xFFFF);
  tft.print("Drinks");
  tft.setCursor(115, 7);
  tft.setTextColor(0);
  tft.print("Pumps");
  tft.drawFastHLine(225, 9, 12, 0xFFFF);
  tft.fillTriangle(225, 5, 225, 13, 220, 9, 0xFFFF);
  tft.drawFastHLine(220, 19, 12, 0xFFFF);
  tft.fillTriangle(232, 15, 232, 23, 238, 19, 0xFFFF);
  tft.fillCircle(205, 14, 6, 0x551F);
  tft.fillCircle(205, 14, 3, 0);

  //Bottom
  if(adminMode)
  {
    tft.fillRect(0, 290, tftWidth, tftHeight - 290, 0xDEFB);
    tft.setTextSize(1);
    tft.setTextColor(0);
    tft.setCursor(35, 302);
    tft.print("Pour Drink");
    tft.setCursor(150, 302);
    tft.print("Loaded Drink");
    tft.fillCircle(135, 305, 6, 0xFD60);
    tft.fillCircle(135, 305, 3, 0xDEFB);
    tft.drawFastVLine(20, 295, 10, 0xFBA0);
    tft.fillTriangle(15, 305, 25, 305, 20, 310, 0xFBA0);
    tft.drawFastHLine(15, 310, 11, 0xFBA0);
  }
  
  //Options
  tft.setTextSize(2);
  tft.setTextColor(0x06E1);
  for (int i = 1; i <= numberOfPumps; i++)
  {
    tft.setCursor(40, 20 + i * 20);
    tft.print("Pump ");
    tft.print(i);
  }
  if(adminMode)
  {
    for (int i = 1; i <= numberOfDrinkSizes; i++)
    {
      if(i == selectedQuickSize)
        tft.setTextColor(0xFFFF);
      else
        tft.setTextColor(0xDB60);
      tft.setCursor(40, 20 + numberOfPumps * 20 + 20 * i);
      tft.print("Size ");
      tft.print(i);
    }
    tft.setTextColor(0xDB60);
    tft.setCursor(40, 20 + numberOfPumps * 20 + 20 * numberOfDrinkSizes + 20);
    tft.print("Cal/Sizes");
  }
  

  //Add Values
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  for (int i = 1; i <= numberOfPumps; i++)
  {
    tft.setCursor(140, 24 + i * 20);
    tft.print(drinksLoaded[i - 1]);
  }
  if(adminMode)
  {
    for (int i = 1; i <= numberOfDrinkSizes; i++)
    {
      tft.setCursor(140, 24 + numberOfPumps * 20 + 20 * i);
      tft.print(drinkSizes[i-1]);
      tft.setCursor(162, 24 + numberOfPumps * 20 + 20 * i);
      tft.print(" centiliter");
    }
  }
  
  
  //Add Arrow
  showArrow(10, 26, 20, currentPumpSelected, 15);

  if(adminMode)
    MaxEncoderValue = encoderSensitivity * (numberOfPumps + numberOfDrinkSizes + 1);
  else
    MaxEncoderValue = encoderSensitivity * numberOfPumps;
  MinEncoderValue = 0;
   
  encoderValue = MinEncoderValue;
  calSizeActive = false;
  encoderTurned = false;
}

void updatePumpSelected()
{
  encoderTurned = false;
  byte newEncoderValue = map(encoderValue, MinEncoderValue, MaxEncoderValue, 1, MaxEncoderValue/encoderSensitivity);

  if (currentPumpSelected == newEncoderValue) //If no change, then no nead to draw again
    return;

  //Clear Previous Arrow
  hideArrow(10, 26, 20, currentPumpSelected, 15);

  //Update CurrentPumpSelected
  currentPumpSelected = newEncoderValue;

  //Draw New Arrow
  showArrow(10, 26, 20, currentPumpSelected, 15);
}

void updateQuickSize()
{
  updateQuickSizeSelected = false;
  tft.setTextSize(2);
  if(selectedQuickSize == currentPumpSelected-numberOfPumps)
  {
    tft.setTextColor(0xDB60);
    tft.setCursor(40, 20 + numberOfPumps * 20 + 20 * selectedQuickSize);
    tft.print("Size ");
    tft.print(selectedQuickSize);
    selectedQuickSize = 0;
  }
  else
  {
    if(selectedQuickSize != 0)
    {
      tft.setTextColor(0xDB60);
      tft.setCursor(40, 20 + numberOfPumps * 20 + 20 * selectedQuickSize);
      tft.print("Size ");
      tft.print(selectedQuickSize);
    }
    
    selectedQuickSize = currentPumpSelected-numberOfPumps;
    
    tft.setTextColor(0xFFFF);
    tft.setCursor(40, 20 + numberOfPumps * 20 + 20 * selectedQuickSize);
    tft.print("Size ");
    tft.print(selectedQuickSize);
  } 
}

void toggleCalSize()
{
  tft.setTextSize(2);
  if(!calSizeActive) //Active
  {
    calSizeActive = true;
    tft.setTextColor(0xFFFF);
    tft.setCursor(40, 40 + (numberOfPumps+numberOfDrinkSizes) * 20);
    tft.print("Cal/Sizes");
  }
  else //OFF
  {
    calSizeActive = false;
    tft.setTextColor(0xDB60);
    tft.setCursor(40, 40 + (numberOfPumps+numberOfDrinkSizes) * 20);
    tft.print("Cal/Sizes");
  }
  calSizeStart = false;  
}

void changeSize()
{ 
  unsigned short drinkSize = drinkSizes[currentPumpSelected-1-numberOfPumps];
  encoderValue = drinkSize/2;
  MinEncoderValue = 0;
  MaxEncoderValue = 1000; //5 liters max
  tft.setTextSize(1);
  tft.setTextColor(0x06E1);
  tft.setCursor(140, 24 + numberOfPumps * 20 + 20 * (currentPumpSelected-numberOfPumps));
  tft.print(drinkSize);
      
  while(currentMenu == PUMP_MENU_SIZE_SELECTION)
  {
    if(encoderTurned)
    {
      encoderTurned = false;
      tft.setTextColor(0);
      tft.setCursor(140, 24 + numberOfPumps * 20 + 20 * (currentPumpSelected-numberOfPumps));
      tft.print(drinkSize);
    
      drinkSize = encoderValue*2;
      
      tft.setTextColor(0x06E1);
      tft.setCursor(140, 24 + numberOfPumps * 20 + 20 * (currentPumpSelected-numberOfPumps));
      tft.print(drinkSize);
    }
  }

  tft.setTextColor(0xFFFF);
  tft.setCursor(140, 24 + numberOfPumps * 20 + 20 * (currentPumpSelected-numberOfPumps));
  tft.print(drinkSize);
  drinkSizes[currentPumpSelected-1-numberOfPumps] = drinkSize;
  toggleCalSize();
  MaxEncoderValue = encoderSensitivity * (numberOfPumps + numberOfDrinkSizes + 1);
  encoderValue = currentPumpSelected*encoderSensitivity;
}
