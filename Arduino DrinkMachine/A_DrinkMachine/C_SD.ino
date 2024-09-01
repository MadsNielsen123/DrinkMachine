byte getNextDrinks(char startChar = '*') //if startChar is '*', (Default) then all startchars will be accepted
{
  byte numberOfRecipesFound = 0;
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  if (recipesFile)
  {
    recipesFile.seek(102 * highestSavedDrink); //Skip past previous saved drinks
    String currentRecipe;
    for (int row = 0; row < numberOfSavedDrinks; row++) //runs through 10 finds. If atleast 1 found: clear saved before saving next
    {
      bool newDrinkFound = true;
      currentRecipe = "";
      for (int i = 0; i < 102; i++) //read line from SD to currentIngredient-String, if StartChar doesnt match: don't process.
      {
        if (recipesFile.available())
        {
          if (i == 4 && startChar != '*') //First letter of ingredient name
          {
            byteHolder = recipesFile.read();
            currentRecipe += byteHolder;
            if (byteHolder != startChar)
            {
              newDrinkFound = false;
            }
          }
          else
            currentRecipe += char(recipesFile.read());
        }
        else //If nothing left
        {
          newDrinkFound = false;
          break;
        }
      }

      if (newDrinkFound) //Process & Save Drink
      {
        if (row == 0) //If theres any new recipe found: clear old saved drinks
        {
          memset(savedDrinkNumbers, 0, sizeof(savedDrinkNumbers));
          for (int i = 0; i < numberOfSavedDrinks; i++)
          {
            savedDrinkNames[i] = "";
          }
        }

        numberOfRecipesFound++;

        //Save Number
        savedDrinkNumbers[row] = currentRecipe.substring(0, 4).toInt();

        //Save Drink
        savedDrinkNames[row] = currentRecipe.substring(4, 20);
      }
      else
      {
        row--;
        if (!recipesFile.available()) //If nothing more to read: end
          break;
      }
    }

    recipesFile.close();
    highestSavedDrink = savedDrinkNumbers[0];
    for (int i = numberOfSavedDrinks; i > 0; i--)
    {
      if (savedDrinkNumbers[i - 1] != 0)
      {
        highestSavedDrink = savedDrinkNumbers[i - 1];
        break;
      }
    }
    lowestSavedDrink = savedDrinkNumbers[0];
  }
  else
  {
    tft.fillScreen(0);
    tft.setTextSize(3);
    tft.setCursor(30, 150);
    tft.setTextColor(0xFFFF);
    tft.print("SD ERROR");
    while (1);
  }
  return numberOfRecipesFound;
}

void getPreviousDrinks(char startChar = '*') //if startChar is '*' (Default), then all startchars will be accepted
{

  if (lowestSavedDrink < 2) //No reason to save new drinks if the lowest has been saved already
    return;

  recipesFile = SD.open("DRINKS.TXT"); //Open File
  if (recipesFile)
  {
    recipesFile.seek(102 * (lowestSavedDrink - numberOfSavedDrinks - 1)); //Skip past first drinks
    String currentRecipe;
    for (int row = 0; row < numberOfSavedDrinks; row++) //runs through [numberOfSavedDrinks] finds. If atleast 1 found: clear saved before saving next
    {
      bool drinkFound = true;
      currentRecipe = "";
      for (int i = 0; i < 102; i++) //read line from SD to currentIngredient-String, if StartChar doesnt match: don't process.
      {
        if (recipesFile.available())
        {
          if (i == 4 && startChar != '*') //First letter of ingredient name
          {
            byteHolder = recipesFile.read();
            currentRecipe += byteHolder;
            if (byteHolder != startChar)
            {
              drinkFound = false;
            }
          }
          else
            currentRecipe += char(recipesFile.read());
        }
        else //If nothing left
        {
          drinkFound = false;
          break;
        }
      }

      if (drinkFound) //Process & Save Drink
      {
        if (row == 0) //If theres any new recipe found: clear old saved drinks
        {
          memset(savedDrinkNumbers, 0, sizeof(savedDrinkNumbers));
          for (int i = 0; i < numberOfSavedDrinks; i++)
          {
            savedDrinkNames[i] = "";
          }
        }

        //Save Number
        savedDrinkNumbers[row] = currentRecipe.substring(0, 4).toInt();

        //Save Drink
        savedDrinkNames[row] = currentRecipe.substring(4, 20);
      }
      else
      {
        row--;
        if (!recipesFile.available()) //If nothing more to read: end
          break;
      }    
    }

    recipesFile.close();
    highestSavedDrink = savedDrinkNumbers[0];
    for (int i = numberOfSavedDrinks; i > 0; i--)
    {
      if (savedDrinkNumbers[i - 1] != 0)
      {
        highestSavedDrink = savedDrinkNumbers[i - 1];
        break;
      }
    }
    lowestSavedDrink = savedDrinkNumbers[0];
  }
  else
  {
    tft.fillScreen(0);
    tft.setTextSize(3);
    tft.setCursor(30, 150);
    tft.setTextColor(0xFFFF);
    tft.print("SD ERROR");
    while (1);
  }
}

byte getFirstDrinks(char startChar = '*') //if startChar is '*', (Default) then all startchars will be accepted
{
  highestSavedDrink = 0;
  lowestSavedDrink = 0;
  return getNextDrinks(startChar);
}

byte getNextMakeableDrinks()
{
  if (getNumberOfDrinksLoaded() == 0)
    return 0;
  
  byte numberOfNewRecipesFound = 0;
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  
  if (recipesFile)
  {
    recipesFile.seek(102 * highestSavedDrink); //Skip past previous drinks
    String currentRecipe;
    
    for (int row = 0; row < numberOfMakeableDrinksToGet; row++) //runs through [numberOfMakeableDrinksToGet] finds. If atleast 1 found: clear saved before saving next
    {
      currentRecipe = "";
         
      for (int i = 0; i < 102; i++) //read line from SD to currentIngredient-String, if StartChar doesnt match: don't process.
      {
        if (recipesFile.available())
          currentRecipe += char(recipesFile.read()); //add chars to string
        else //If nothing left
          break;
      }
  
      if (drinkCanBeMade(currentRecipe)) //Process & Save Drink
      {
        if (row == 0) //If found one: clear old saved drinks
        {
          memset(savedDrinkNumbers, 0, sizeof(savedDrinkNumbers));
          for (int i = 0; i < numberOfSavedDrinks; i++)
          {
            savedDrinkNames[i] = "";
          }
        }

        numberOfNewRecipesFound++;       
        savedDrinkNumbers[row] = currentRecipe.substring(0, 4).toInt(); //Save Number    
        savedDrinkNames[row] = currentRecipe.substring(4, 20);  //Save Drink

      }
      else
      {
        row--;
        if (!recipesFile.available()) //If nothing more to read: end
          break;
      }
    }

    recipesFile.close();
    highestSavedDrink = savedDrinkNumbers[0];
    for (int i = numberOfSavedDrinks; i > 0; i--)
    {
      if (savedDrinkNumbers[i - 1] != 0)
      {
        highestSavedDrink = savedDrinkNumbers[i - 1];
        break;
      }
    }
    lowestSavedDrink = savedDrinkNumbers[0];
    return numberOfNewRecipesFound;
  }
  else
  {
    tft.fillScreen(0);
    tft.setTextSize(3);
    tft.setCursor(30, 150);
    tft.setTextColor(0xFFFF);
    tft.print("SD ERROR");
    while (1);
  }
}

void getPreviousMakeableDrinks()
{
  if (getNumberOfDrinksLoaded() == 0)
    return;

  if (lowestSavedDrink < 2) //No reason to save new drinks if the lowest has been saved already
    return;
  
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  
  if (recipesFile)
  {
    recipesFile.seek(102 * (lowestSavedDrink - numberOfMakeableDrinksToGet - 1)); //Skip past first drinks
    String currentRecipe;
    
    for (int row = 0; row < numberOfMakeableDrinksToGet; row++) //runs through [numberOfMakeableDrinksToGet] finds. If atleast 1 found: clear saved before saving next
    {
      currentRecipe = "";
         
      for (int i = 0; i < 102; i++) //read line from SD to currentIngredient-String, if StartChar doesnt match: don't process.
      {
        if (recipesFile.available())
          currentRecipe += char(recipesFile.read()); //add chars to string
        else //If nothing left
          break;
      }
  
      if (drinkCanBeMade(currentRecipe)) //Process & Save Drink
      {
        if (row == 0) //If found one: clear old saved drinks
        {
          memset(savedDrinkNumbers, 0, sizeof(savedDrinkNumbers));
          for (int i = 0; i < numberOfSavedDrinks; i++)
          {
            savedDrinkNames[i] = "";
          }
        }
    
        savedDrinkNumbers[row] = currentRecipe.substring(0, 4).toInt(); //Save Number    
        savedDrinkNames[row] = currentRecipe.substring(4, 20);  //Save Drink

      }
      else
      {
        row--;
        if (!recipesFile.available()) //If nothing more to read: end
          break;
      }
    }

    recipesFile.close();
    highestSavedDrink = savedDrinkNumbers[0];
    for (int i = numberOfSavedDrinks; i > 0; i--)
    {
      if (savedDrinkNumbers[i - 1] != 0)
      {
        highestSavedDrink = savedDrinkNumbers[i - 1];
        break;
      }
    }
    lowestSavedDrink = savedDrinkNumbers[0];  
  }
  else
  {
    tft.fillScreen(0);
    tft.setTextSize(3);
    tft.setCursor(30, 150);
    tft.setTextColor(0xFFFF);
    tft.print("SD ERROR");
    while (1);
  } 
  
}

byte getFirstMakeableDrinks()
{
  highestSavedDrink = 0;
  lowestSavedDrink = 0;
  return getNextMakeableDrinks();
}

byte getNumberOfDrinksLoaded()
{
  byte count = 0;
  for (int i = 0; i < numberOfPumps; i++)
  {
    if (drinksLoaded[i] != "" && !previousPumpLoadedWithSameDrink(i))
      count++;
  }
  numberOfDrinksLoaded = count;
  return count;
}

bool drinkCanBeMade(String recipe)
{
  unsigned int currentIngredient;
  bool ingredientIsLoaded;
  bool actualIngreedientsIsRequired = false;
  for(byte ingredientNumber = 0; ingredientNumber<numberOfPumps; ingredientNumber++) //0-7
  {
    ingredientIsLoaded = false;
    currentIngredient = recipe.substring(25+ingredientNumber*10, 25+ingredientNumber*10+4).toInt();
    if(currentIngredient != 0)
      actualIngreedientsIsRequired = true;

    for(byte loadedDrinkNumber = 0; loadedDrinkNumber < numberOfPumps; loadedDrinkNumber++) //0-7
    {
      if(currentIngredient != 0 && currentIngredient == drinksLoadedNumbers[loadedDrinkNumber])
        ingredientIsLoaded = true;      
      else if(currentIngredient == 0)
        ingredientIsLoaded = true;
    }
    if(!ingredientIsLoaded)
      return false;
  }
  if(actualIngreedientsIsRequired) //If all ingreedients has number 0000 then drink should be ignored
    return true;
  else
    return false;
}

String getRecipe(unsigned int recipeNumber)
{
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  if (recipesFile)
  {
    recipesFile.seek(102 * (recipeNumber - 1)); //Skip past first drinks
    String recipe;
    for (int i = 0; i < 102; i++)
    {
      if (recipesFile.available())
        recipe += char(recipesFile.read());
    }
    recipesFile.close();
    return recipe;
  }
  else
  {
    tft.fillScreen(0);
    tft.setTextSize(3);
    tft.setCursor(30, 150);
    tft.setTextColor(0xFFFF);
    tft.print("SD ERROR");
    while (1);
  }
  return "";
}
