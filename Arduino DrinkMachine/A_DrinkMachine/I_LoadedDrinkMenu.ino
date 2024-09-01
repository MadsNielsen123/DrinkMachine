void changeSearchMode(bool justEnteredMenu = false) //Changes Search Mode and load the first drinks (according to the search mode)
{
  changeSearchModeFlag = false;
  alphabeticalSearch = !alphabeticalSearch;
  encoderPressed = false;
  hideArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);

  if (alphabeticalSearch)
  {
    tft.drawFastHLine(90, 88, 33, 0); //Clear Line Under "All"
    tft.drawFastHLine(193, 88, 15, 0xFBA0); //Draw Line Under Character
    if(!justEnteredMenu)
      chooseSeachChar(); //loops until char is selected or bluebtn-return
    currentPage = 1;
    byte numberOfFoundDrinks = getFirstDrinks(alphabet[numberOfSelectedSearchChar]);
    MaxEncoderValue = encoderSensitivity * (numberOfFoundDrinks + 1); //Loads drinks and sets max encoder val from that   
    encoderValue = MinEncoderValue;
    encoderTurned = false;
    currentLoadedDrinkOptionSelected = 1;
    encoderTriedToPassMin = false;
    encoderTriedToPassMax = false;
    printFirstPage(numberOfFoundDrinks);
    showArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);
  }
  else
  {
    tft.drawFastHLine(193, 88, 15, 0); //Clear Line Under Character
    tft.drawFastHLine(90, 88, 33, 0xFBA0); //Draw Line under "All"
    currentPage = 1;
    byte numberOfFoundDrinks = getFirstDrinks();
    MaxEncoderValue = encoderSensitivity * (numberOfFoundDrinks + 1);
    encoderValue = MinEncoderValue;
    encoderTurned = false;
    currentLoadedDrinkOptionSelected = 1;
    encoderTriedToPassMin = false;
    encoderTriedToPassMax = false;
    printFirstPage(numberOfFoundDrinks);
    showArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);
  }
}

void loadedDrinkMenu()
{
  loadedDrinkMenuSetup();

  while (currentMenu == LOADED_DRINK_MENU)
  {
    if (encoderPressed)
      updateLoadedDrink();
      
    if (encoderTurned) 
      updateLoadedDrinkOptions();

    if (changeSearchModeFlag)
      changeSearchMode();
  }
  getNumberOfDrinksLoaded();
}

 
void loadedDrinkMenuSetup()
{
  currentPage = 1;
  MinEncoderValue = 1;  
  tft.fillScreen(0);

  //Show Pump Title
  tft.setTextSize(3);
  tft.setCursor(15, 5);
  tft.setTextColor(0xFFFF);
  tft.print("Pump ");
  tft.print(currentPumpSelected);
  tft.drawFastHLine(0, 38, tftWidth, 0xFFFF);
  tft.drawFastHLine(0, 95, tftWidth, 0xFFFF);

  //Instructions
  tft.drawFastHLine(215, 10, 20, 0xFFFF);
  tft.fillTriangle(215, 6, 215, 14, 210, 10, 0xFFFF);
  tft.fillCircle(223, 25, 6, 0x551F);
  tft.fillCircle(223, 25, 3, 0);

  //Show Loaded Drink & Search mode
  tft.setTextSize(2);
  tft.setCursor(5, 45);
  tft.setTextColor(0xFBA0);
  tft.print("Loaded: ");
  tft.setTextColor(0x06E1);
  tft.setCursor(90, 45);
  if(drinksLoaded[currentPumpSelected - 1] == "")
    tft.print("None");
  else
    tft.print(drinksLoaded[currentPumpSelected - 1]);
  
  //updateLoadedDrink();
  tft.setCursor(5, 70);
  tft.setTextColor(0xFBA0);
  tft.print("Search: ");
  tft.setCursor(90, 70);
  tft.setTextColor(0x06E1);
  tft.print("All  /");
  tft.setCursor(195, 70);
  tft.print(alphabet[numberOfSelectedSearchChar]);
  alphabeticalSearch = !alphabeticalSearch;
  changeSearchMode(true);

  //Draw Arrow
  currentLoadedDrinkOptionSelected = 1;
  showArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);
}

void updateLoadedDrink()
{
  encoderPressed = false;
  if(currentPage == 1 && currentLoadedDrinkOptionSelected == 1 && drinksLoaded[currentPumpSelected - 1] == "")
    return; //None was already selected - no need to update  
  else if(currentPage == 1 && drinksLoaded[currentPumpSelected - 1] == savedDrinkNames[currentLoadedDrinkOptionSelected - 2])
    return; //Drink on first page was already selected - no need to update
  else if(currentPage != 1 && drinksLoaded[currentPumpSelected - 1] == savedDrinkNames[currentLoadedDrinkOptionSelected - 1])
    return; //Drink was already selected - no need to update

  //Clear Previous Loaded Drink
  tft.setTextColor(0);
  tft.setCursor(90, 45);
  if(drinksLoaded[currentPumpSelected - 1] == "")
    tft.print("None");
  else
    tft.print(drinksLoaded[currentPumpSelected - 1]);
    
  //Update and display Loaded Drink
  tft.setTextColor(0x06E1);
  tft.setCursor(90, 45);
  if(currentPage == 1 && currentLoadedDrinkOptionSelected == 1)//None is choosen
  {
    drinksLoaded[currentPumpSelected - 1] = "";
    drinksLoadedNumbers[currentPumpSelected - 1] = 0;
    tft.print("None");   
  } 
  else if (currentPage == 1)
  {
    drinksLoaded[currentPumpSelected - 1] = savedDrinkNames[currentLoadedDrinkOptionSelected - 2];
    drinksLoadedNumbers[currentPumpSelected - 1] = savedDrinkNumbers[currentLoadedDrinkOptionSelected - 2];
    tft.print(drinksLoaded[currentPumpSelected - 1]);
  }
  else
  {
    drinksLoaded[currentPumpSelected - 1] = savedDrinkNames[currentLoadedDrinkOptionSelected - 1];
    drinksLoadedNumbers[currentPumpSelected - 1] = savedDrinkNumbers[currentLoadedDrinkOptionSelected - 1];
    tft.print(drinksLoaded[currentPumpSelected - 1]);
  }
}

void chooseSeachChar()
{
  MaxEncoderValue = encoderSensitivity * 25; //25(+1 starts at 0) is 26... the size of the alphabet
  MinEncoderValue = 0;
  encoderValue = encoderSensitivity * numberOfSelectedSearchChar;
  encoderTurned = false;
  encoderPressed = false;

  tft.setTextColor(0xFFFF);
  tft.setCursor(195, 70);
  tft.print(alphabet[numberOfSelectedSearchChar]);

  while (currentMenu == LOADED_DRINK_MENU)
  {
    if (encoderTurned) //Update Character
    {
      encoderTurned = false;
      byte newEncoderValue = map(encoderValue, MinEncoderValue, MaxEncoderValue, 0, 25); //Maps the Encoder Value (0-50) to the Alphabet (0-25)
      if (numberOfSelectedSearchChar != newEncoderValue) //Only draw, if there is a change
      {
        //Clear Previous Char
        tft.setTextColor(0);
        tft.setCursor(195, 70);
        tft.print(alphabet[numberOfSelectedSearchChar]);

        //Update CurrentPumpSelected
        numberOfSelectedSearchChar = newEncoderValue;

        //Draw New Char
        tft.setTextColor(0xFFFF);
        tft.setCursor(195, 70);
        tft.print(alphabet[numberOfSelectedSearchChar]);
      }
    }

    if (encoderPressed) //If EncoderPressed Continue with chosen character
    {
      tft.setTextColor(0x06E1);
      tft.setCursor(195, 70);
      tft.print(alphabet[numberOfSelectedSearchChar]);
      encoderPressed = false;
      changeSearchModeFlag = false; //just in case
      break;
    }
  }
}

void updateLoadedDrinkOptions()
{
  encoderTurned = false;
  byte newEncoderValue;
  newEncoderValue = map(encoderValue, MinEncoderValue-1, MaxEncoderValue, 1, MaxEncoderValue/encoderSensitivity);

  if (currentLoadedDrinkOptionSelected == newEncoderValue && !encoderTriedToPassMin && !encoderTriedToPassMax) //If no change & it didnt try to change page, then no need to do anything
    return;

  hideArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);

  if (encoderTriedToPassMin && currentPage != 1) //GoTo Previous Page
  {
    Serial.println("Previous Page");
    currentPage--;
    encoderTriedToPassMin = false; 
    encoderTriedToPassMax = false;  
    if (currentPage == 1)
      MaxEncoderValue = encoderSensitivity * (numberOfSavedDrinks + 1);
    else
      MaxEncoderValue = encoderSensitivity * numberOfSavedDrinks;
    newEncoderValue = MaxEncoderValue/encoderSensitivity;
    encoderValue = MaxEncoderValue;
    printPreviousPage();
  }
  else if (encoderTriedToPassMax && MaxEncoderValue >= numberOfSavedDrinks) //GoTo Next Page
  {
    encoderTriedToPassMin = false; 
    encoderTriedToPassMax = false;      
    byte numberOfNewDrinksFound = checkNextDrinks();
    
    if (numberOfNewDrinksFound > 0)
    {
      Serial.println("Next Page");
      currentPage++;
      newEncoderValue = 1;
      encoderValue = MinEncoderValue;
      MaxEncoderValue = numberOfNewDrinksFound * encoderSensitivity;
      printNextPage();
    }
    else
    {
      newEncoderValue = MaxEncoderValue/encoderSensitivity;
      encoderValue = MaxEncoderValue;
    }
  }


  currentLoadedDrinkOptionSelected = newEncoderValue; //Update Selected Option
  showArrow(2, 85, 20, currentLoadedDrinkOptionSelected, 10);
}

byte checkNextDrinks() //Checks if theres more drinks - returns number of next drinks
{
  if (alphabeticalSearch)
  {
    return getNextDrinks(alphabet[numberOfSelectedSearchChar]);
  }
  else
  {
    return getNextDrinks();
  }
}

void printNextPage()
{
  tft.fillRect(0, 100, tftWidth, tftHeight - 100, 0);
  for (int i = 0; i < numberOfSavedDrinks; i++)
  {
    tft.setTextColor(0x06E1);
    tft.setCursor(25, 100 + 20 * i);
    tft.print(savedDrinkNames[i]);
  }
}

void printPreviousPage()
{
  if (alphabeticalSearch)
    getPreviousDrinks(alphabet[numberOfSelectedSearchChar]);
  else
    getPreviousDrinks();

  if (currentPage == 1)
    printFirstPage(1);
  else
  {
    tft.fillRect(0, 100, tftWidth, tftHeight - 100, 0);
    tft.setTextColor(0x06E1);
    for (int i = 0; i < numberOfSavedDrinks; i++)
    {     
      tft.setCursor(25, 100 + 20 * i);
      tft.print(savedDrinkNames[i]);
    }
  }
}

void printFirstPage(byte numberOfFoundDrinks)
{
  tft.fillRect(0, 100, tftWidth, tftHeight - 100, 0);
  tft.setTextColor(0xFFFF);
  tft.setCursor(25, 100);
  tft.print("None");
  if(numberOfFoundDrinks > 0)
  {
    tft.setTextColor(0x06E1);
    for (int i = 0; i < numberOfSavedDrinks; i++)
    {   
      tft.setCursor(25, 120 + 20 * i);
      tft.print(savedDrinkNames[i]);
    }
  }  
}

bool previousPumpLoadedWithSameDrink(byte pumpNumber)
{
  for(int i = 0; i<pumpNumber; i++)
  {
    if(drinksLoadedNumbers[pumpNumber] == drinksLoadedNumbers[i])
      return true;
  }
  return false;
}
