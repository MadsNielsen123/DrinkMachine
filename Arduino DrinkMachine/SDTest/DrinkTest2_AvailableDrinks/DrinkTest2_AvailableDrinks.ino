/* NOTES
   1. Takes numbers of the drinks that is loaded - into an array[8] (should be read from sd and selected in the future)
   2. Tells what other drinks can be made with those
*/

#include <SPI.h>
#include <SD.h>
File recipesFile;
const int chipSelect = 4;
char lastByteRead = ' ';
unsigned int numberOfRecipes;
String lastDrinkInfo;
const byte numberOfPumps = 8;
unsigned int loadedDrinks[numberOfPumps] = {0, 0, 1, 2 , 0 , 0 , 0 , 0};

void setup()
{

  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  printAllDrinks();

  printAvailableDrinks();

}

void loop()
{


}

bool printAllDrinks()
{
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  if (recipesFile)
  {
    Serial.println("Drinks available to select: ");
    numberOfRecipes = 0;
    lastDrinkInfo = "";
    // read from the file until there's nothing else in it:
    while (recipesFile.available())
    {
      Serial.print(++numberOfRecipes);
      Serial.print(" ");
      while (lastByteRead != '\n' && recipesFile.available()) //until end of line or no more data
      {
        lastByteRead = recipesFile.read();
        lastDrinkInfo += lastByteRead; //store drinkinfo into string
      }
      Serial.println(lastDrinkInfo.substring((lastDrinkInfo.indexOf('"') + 1), (lastDrinkInfo.lastIndexOf('"'))));
      lastByteRead = recipesFile.read();
      lastDrinkInfo = "";
    }
  }
  else // if the file didn't open, print an error and return false
  {
    Serial.println("error opening drink_recipes.txt");
    recipesFile.close();
    return false;
  }

  recipesFile.close(); //close file
  return true;

}

void printAvailableDrinks()
{
  recipesFile = SD.open("DRINKS.TXT"); //Open File
  if (recipesFile)
  {
    lastDrinkInfo = "";
    Serial.println("Drinks available to make: ");

    // read from the file until there's nothing else in it:
    while (recipesFile.available())
    {
      while (lastByteRead != '\r' && recipesFile.available()) //store drinkinfo into string
      {
        lastByteRead = recipesFile.read();
        lastDrinkInfo += lastByteRead;
      }

      // --------------  Check if drink itself is loaded -------------------------
      bool drinkIsLoaded = false;
      for (int i = 0; i < numberOfPumps; i++)
      {
        if (lastDrinkInfo.substring((lastDrinkInfo.indexOf('.') - 1), lastDrinkInfo.indexOf('.')).toInt() == loadedDrinks[i])
        {
          drinkIsLoaded = true;
        }
      }


      // --------------  Check if loaded drinks can make this recipe. If so - print them
      //1. Count Ingredients (how mane %'s)
      byte numberOfIngredients = 0;
      for (int i = 0; i < lastDrinkInfo.length(); i++)
      {
        if (lastDrinkInfo[i] == '%')
          numberOfIngredients++;
      }
      //2. Check every ingredientnumber and if they are loaded
      bool allIngredientsIsLoaded = true;
      unsigned int previousCharIndex = -1;
      for (int i = 0; i < numberOfIngredients; i++)
      {
        bool currentIngredientIsLoaded = false;
        unsigned int charIndex = (lastDrinkInfo.indexOf('%', previousCharIndex + 1));
        for (int e = 0; e < numberOfPumps; e++)
        {
          if (lastDrinkInfo.substring(charIndex + 1).toInt() == loadedDrinks[e])
          {
            currentIngredientIsLoaded = true;
          }
        }
        previousCharIndex = charIndex;
        if (!currentIngredientIsLoaded)
          allIngredientsIsLoaded = false;
      }

      //3. Print if ingredietns is loaded
      if (allIngredientsIsLoaded && numberOfIngredients != 0 || drinkIsLoaded)
        Serial.println(lastDrinkInfo.substring((lastDrinkInfo.indexOf('"') + 1), (lastDrinkInfo.lastIndexOf('"'))));

      lastByteRead = recipesFile.read();
      lastDrinkInfo = "";
    }
  }
  else // if the file didn't open, print an error and return false
  {
    Serial.println("error opening drink_recipes.txt");
    recipesFile.close();
  }
  recipesFile.close(); //close file
}
