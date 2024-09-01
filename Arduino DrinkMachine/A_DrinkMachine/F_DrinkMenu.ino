void drinkMenu()
{
  drinkMenuSetup();

  if(numberOfDrinksLoaded == 0)
    while(currentMenu == DRINK_MENU); //Do nothing
  else
    while (currentMenu == DRINK_MENU)
    {      
      if (encoderTurned)
        updateDrinkSelected();

      if (encoderPressed)
        currentMenu = SIZE_MENU;

      if(runNextLight)
        nextLight();
    }
}

void drinkMenuSetup()
{
  tft.fillScreen(0); //Clear screen

  //TOP
  tft.drawFastHLine(0, 30, tftWidth, 0xFFFF);
  tft.drawFastVLine(96, 0, 30, 0xFFFF);
  tft.drawFastVLine(192, 0, 30, 0xFFFF);
  tft.fillRect(0, 0, 96, 30, 0x551F);
  tft.setTextSize(2);
  tft.setCursor(10, 7);
  tft.setTextColor(0);
  tft.print("Drinks");
  tft.setCursor(115, 7);
  tft.setTextColor(0xFFFF);
  tft.print("Pumps");
  tft.drawFastHLine(225, 9, 12, 0xFFFF);
  tft.fillTriangle(225, 5, 225, 13, 220, 9, 0xFFFF);
  tft.drawFastHLine(220, 19, 12, 0xFFFF);
  tft.fillTriangle(232, 15, 232, 23, 238, 19, 0xFFFF);
  tft.fillCircle(205, 14, 6, 0x551F);
  tft.fillCircle(205, 14, 3, 0);
  
  //Test
  if(numberOfDrinksLoaded == 0)
  {
    tft.setCursor(25, 160);
    tft.setTextColor(0xFFFF);
    tft.print("No Loaded Drinks");
    return;
  }

  
  printFirstDrinkPage();
  
  currentDrinkOptionSelected = 1;
  showArrow(2, 26, 25, 1, 10);
     
  MinEncoderValue = 1;
  encoderValue = MinEncoderValue;
  encoderTurned = false;
  encoderPressed = false;
  encoderTriedToPassMin = false;
  encoderTriedToPassMax = false;
  currentPage = 1;
}

void updateDrinkSelected()
{
  byte newEncoderValue;
  newEncoderValue = map(encoderValue, MinEncoderValue-1, MaxEncoderValue, 1, MaxEncoderValue/encoderSensitivity);
  
  if (currentDrinkOptionSelected == newEncoderValue && !encoderTriedToPassMin && !encoderTriedToPassMax) //If no change & it didnt try to change page, then no need to do anything
    return;

  hideArrow(2, 26, 25, currentDrinkOptionSelected, 10);
  
  if (encoderTriedToPassMin && currentPage != 1) //GoTo Previous Page
  {
    Serial.println("Previous Page");
    currentPage--;
    encoderTriedToPassMin = false; 
    encoderTriedToPassMax = false;  
    printPreviousDrinkPage();
    newEncoderValue = MaxEncoderValue/encoderSensitivity;
    encoderValue = MaxEncoderValue-1;
  }
  else if (encoderTriedToPassMax && MaxEncoderValue >= numberOfSavedDrinks) 
  {
    encoderTriedToPassMax = false;
    encoderTriedToPassMin = false;
    numberOfMakeableDrinksToGet = numberOfSavedDrinks;     
    byte numberOfNewDrinksFound = getNextMakeableDrinks();
    if (numberOfNewDrinksFound > 0) //GoTo Next Page
    {
      Serial.println("Next Page");
      currentPage++;
      newEncoderValue = 1;
      encoderValue = MinEncoderValue;
      MaxEncoderValue = numberOfNewDrinksFound * encoderSensitivity;
      printNextDrinkPage(numberOfNewDrinksFound);
    }
    else //No new found - stay on current page
    {
      newEncoderValue = MaxEncoderValue/encoderSensitivity;
      encoderValue = MaxEncoderValue;
    }
  }
  currentDrinkOptionSelected = newEncoderValue; //Update Selected Option
  showArrow(2, 26, 25, currentDrinkOptionSelected, 10);
  
  encoderTurned = false;
}

void printNextDrinkPage(byte numberOfNewDrinksFound)
{
  tft.fillRect(0, 31, tftWidth, tftHeight - 31, 0);
  tft.setTextColor(0x06E1);
  for (int i = 0; i < numberOfNewDrinksFound; i++)
  {    
    tft.setCursor(25, 20 + (i+1) * 25);
    tft.print(savedDrinkNames[i]);
  }
  printPageCounter();
}

void printPreviousDrinkPage()
{
  getPreviousMakeableDrinks();
  
  if (currentPage == 1)
  {
    tft.fillRect(0, 31, tftWidth, tftHeight - 31, 0);
    printFirstDrinkPage();
  }
  else
  {
    MaxEncoderValue = encoderSensitivity * numberOfSavedDrinks;
    tft.fillRect(0, 31, tftWidth, tftHeight - 31, 0);
    tft.setTextColor(0x06E1);
    for (int i = 0; i < numberOfSavedDrinks; i++)
    {     
      tft.setCursor(25, 20 + (i+1) * 25);
      tft.print(savedDrinkNames[i]);
    }
    printPageCounter();
  }
}

void printFirstDrinkPage()
{
  byte counter = 0;
  numberOfMakeableDrinksToGet = numberOfSavedDrinks-numberOfDrinksLoaded;
  tft.setTextColor(0x06E1);
  byte numberOfMakeableDrinks = getFirstMakeableDrinks(); //Put them into savedDrinks array
  //Display options
  for(byte i = 0; i<numberOfPumps; i++) //print all the loaded drinks
  {
    if(drinksLoadedNumbers[i] != 0 && !previousPumpLoadedWithSameDrink(i))
    {
      counter++;
      tft.setCursor(25, 20 + counter * 25);    
      tft.print(drinksLoaded[i]);    
    }     
  }
  if(numberOfMakeableDrinks > 0)
  {
    for(byte i = 0; i<numberOfMakeableDrinksToGet; i++) //print all the makeable drinks
    {
      if(savedDrinkNumbers[i] != 0)
      {
        counter++;
        tft.setCursor(25, 20 + counter * 25);    
        tft.print(savedDrinkNames[i]);
      }    
    }
  }
  
  printPageCounter();
  MaxEncoderValue = (encoderSensitivity*counter);
}

void printPageCounter()
{
  tft.setTextSize(1);
  tft.setTextColor(0xFFFF);
  tft.setCursor(190, 300);    
  tft.print("Page ");
  tft.print(currentPage);
  tft.setTextSize(2);
}

void lauchSelectedDrink()
{ 
  
  encoderPressed = false;
  if(currentPage == 1)
  {
    if(currentDrinkOptionSelected <= numberOfDrinksLoaded) //Then its one of the loaded drinks
    {
      short loadedCounter = 0;
      for(int i = 0; i<numberOfPumps; i++) //Run thought the loaded drinks, until it finds the right one
      {
        if(drinksLoadedNumbers[i] != 0)
          loadedCounter++;

        if(previousPumpLoadedWithSameDrink(i))
          currentDrinkOptionSelected++;
                
        if(loadedCounter == currentDrinkOptionSelected) //Then its the right drink
        {
          drinkLauncher(drinksLoaded[currentDrinkOptionSelected-1], drinksLoadedNumbers[currentDrinkOptionSelected-1], true);
          break;
        }
      }
      return;
    }
    else
      drinkLauncher(savedDrinkNames[currentDrinkOptionSelected-numberOfDrinksLoaded-1], savedDrinkNumbers[currentDrinkOptionSelected-numberOfDrinksLoaded-1], false);    
  }
  else
    drinkLauncher(savedDrinkNames[currentDrinkOptionSelected-1], savedDrinkNumbers[currentDrinkOptionSelected-1], false);
}
