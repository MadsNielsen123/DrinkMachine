byte pumpProcentages[numberOfPumps] = {0, 0, 0, 0, 0, 0, 0, 0}; //Sorted after pumps... [0] is pump 1 procentage
bool pumpContainsStrongIngredient[numberOfPumps] = {0, 0, 0, 0, 0, 0, 0, 0};
byte totalStrongIngredientProcentage = 0;
unsigned int totalDrinkSize = drinkSizes[0];
const byte strengthTickValue = 5;
short tickPlace = 3; //0-5 [3 is 0 strenght]

void drinkLauncher(String drinkName, unsigned int drinkNumber, bool aLoadedDrink)
{ 
  adminMode = digitalRead(MODE_BTN);
  drinkLauncherSetup(drinkName);
  memset(pumpProcentages, 0, sizeof(pumpProcentages));
  memset(pumpContainsStrongIngredient, 0, sizeof(pumpContainsStrongIngredient));
  totalStrongIngredientProcentage = 0;
  if(selectedQuickSize == 0 || adminMode)
    totalDrinkSize = getDrinkSizeMenu(); //Select how many centiliters
  else
    totalDrinkSize = drinkSizes[selectedQuickSize-1];
      
  if(aLoadedDrink && !returnFromDrinkLaucher) //--- If ex. two pumps are loaded with a drink, give each 50%
  {  
    byte pumpsWithLoadedDrink = getDrinkPumpNumbers(drinkNumber); //[bit 0] is pump 1
    if(pumpsWithLoadedDrink == 0)
      return;
    else 
    {
      setPumpProcentages(pumpsWithLoadedDrink, 100, false);
    }  
      if(!returnFromDrinkLaucher && secureProcentage())
        mixDrink(drinkName, 0);   
  }
  else if(!returnFromDrinkLaucher)
  {  
    short strength = getDrinkStrengthMenu(); 
    
    if(!returnFromDrinkLaucher && getPumpProcentages(drinkNumber) && strength != 0)
      modifyPumpProcentages(strength);
    
    if(!returnFromDrinkLaucher && secureProcentage())
      mixDrink(drinkName, strength);
  }

   returnFromDrinkLaucher = false;
   adminMode = false;
   currentMenu = DRINK_MENU;
}

byte getDrinkPumpNumbers(unsigned int drinkNumber)
{
  byte byteToReturn = 0;
  for(byte i = 0; i<numberOfPumps; i++) //Go Through each pump
  {
    if(drinkNumber == drinksLoadedNumbers[i])
      byteToReturn |= (1<<i);
  }
  return byteToReturn;
}

byte getNumberOfPumpsWithLoadedDrink(byte pumpsWithLoadedDrink)
{
  byte count = 0;
  for(int i = 0; i<numberOfPumps; i++)
  {
    if(bitRead(pumpsWithLoadedDrink, i) == 1)
      count++;     
  }
  return count;
}

bool getPumpProcentages(unsigned int drinkNumber)
{ 
  String recipe = getRecipe(drinkNumber);
  unsigned short currentIngredient = 0;
  bool currentIngredientIsStrong = false;
  byte pumpsWithLoadedDrink;
  byte numberOfPumpsWithLoadedDrink;
  
  for(byte ingredient = 0; ingredient<numberOfPumps; ingredient++) //Go through all ingreedients
  {
    currentIngredient = getNumberOfIngredient(recipe, ingredient);
    if(currentIngredient == 0)
      continue;
    else
    {
      pumpsWithLoadedDrink = getDrinkPumpNumbers(currentIngredient); //[bit 0] is pump 1
      if(pumpsWithLoadedDrink == 0)
        return false;

      currentIngredientIsStrong = ingredientIsStrong(recipe, ingredient);
      setPumpProcentages(pumpsWithLoadedDrink, getIngredientProcentage(recipe, ingredient), currentIngredientIsStrong);
    }   
  }

  return true;
}

void modifyPumpProcentages(short strength)
{
  if(totalStrongIngredientProcentage == 0 || totalStrongIngredientProcentage == 100)
    return;
  
  for(int i = 0; i<numberOfPumps; i++)
  {
    if(pumpProcentages[i] == 0)
      continue;

    if(pumpContainsStrongIngredient[i])  
      pumpProcentages[i] += round(float(pumpProcentages[i]*strength)/totalStrongIngredientProcentage);
    else   
      pumpProcentages[i] -= round(float(pumpProcentages[i]*strength)/(100-totalStrongIngredientProcentage));
      
    if(pumpProcentages[i]<1)
      pumpProcentages[i] = 1;
    else if(pumpProcentages[i]> 99)
      pumpProcentages[i] = 99;
  }
}

bool ingredientIsStrong(String recipe, byte ingredient)
{
  if(recipe[20+ingredient*10] == '[')
    return true;
  else
    return false;
}

void setPumpProcentages(byte pumpsWithLoadedDrink, byte ingredientProcentage, bool currentIngredientIsStrong)
{
  byte numberOfPumpsWithLoadedDrink = getNumberOfPumpsWithLoadedDrink(pumpsWithLoadedDrink);
  for(byte i = 0; i<numberOfPumps; i++)
  {
    if(bitRead(pumpsWithLoadedDrink, i) == 1)
    {
      pumpProcentages[i] = ingredientProcentage/numberOfPumpsWithLoadedDrink;
      pumpContainsStrongIngredient[i] = currentIngredientIsStrong;
    }
  }
}

unsigned short getNumberOfIngredient(String recipe, byte ingredient)
{
  return recipe.substring(25+10*ingredient, 29+10*ingredient).toInt();
}

byte getIngredientProcentage(String recipe, byte ingredient)
{
  byte procentage = recipe.substring(21+10*ingredient, 24+10*ingredient).toInt();
  if(ingredientIsStrong(recipe, ingredient))
    totalStrongIngredientProcentage += procentage;
  return procentage;
}

void mixDrink(String drinkName, short strength)
{
  currentMenu = MIX_MENU;
  if(!drinkAcceptedMenu(drinkName, strength))
    return;

  //Clear Previous Text, Btns & circles
  tft.setTextSize(3);
  tft.setTextColor(0);
  tft.setCursor(30, 15);
  tft.print("Drink OK ?");
  tft.setTextSize(2);
  tft.setCursor(59, 245);
  tft.print("NO");
  tft.setCursor(153, 245);
  tft.print("YES");
  tft.fillCircle(70, 280, 15, 0);
  tft.fillCircle(170, 280, 15, 0);
  
  
  //Info
  tft.setTextSize(3);
  tft.setTextColor(0x06E1);
  tft.setCursor(70, 15);
  tft.print("MIXING");

  //Bar
  tft.drawRect(19, 280, 202, 20, 0xFFFF);
  
  byte barProcentage = 0;
  float amountOfRecipe = 0;
  float ingredientPoured = 0;
  float currentIngredientPoured = 0;
  float totalPoured = 0;
  byte procentageSum = 0;
  pumpTotalRunTime = 0;
  
  for(byte i = 0; i < numberOfPumps; i++)
  {
    if(pumpProcentages[i] != 0)
      enablePump(i+1);
    else
      continue;

    amountOfRecipe = float(totalDrinkSize*pumpProcentages[i])/100;
    procentageSum += pumpProcentages[i];  
    ingredientPoured = 0;
    currentIngredientPoured = 0;
    startPump();
      
    while(!returnFromDrinkLaucher && ingredientPoured < amountOfRecipe)
    {
      currentIngredientPoured = float(millis() - pumpStartTime) / pumpsCentiliterTime[i]; //measure now
      totalPoured = totalPoured + (currentIngredientPoured-ingredientPoured);
      ingredientPoured = currentIngredientPoured;
      barProcentage = updateBar(barProcentage, totalPoured);
    }    
    stopPump();
     
    while(barProcentage<procentageSum && !returnFromDrinkLaucher) //still missing som bar status
     barProcentage = updateBar(barProcentage, totalPoured); 
    
    if(returnFromDrinkLaucher)
      break;
  }
  disableStepper();
   
  tone(BUZZ_PIN, 1400);
  delay(50);
  noTone(BUZZ_PIN);
  delay(50);
  tone(BUZZ_PIN, 1000, 50);
  delay(1000);
}

byte updateBar(byte barProcentage, float totalPoured)
{
  if( totalPoured > (totalDrinkSize*barProcentage)/100 && barProcentage != 100) //Then increment Bar
  {
    barProcentage++;
    tft.drawFastVLine(18+barProcentage*2, 281, 18, 0x06E1);
    tft.drawFastVLine(19+barProcentage*2, 281, 18, 0x06E1);
  }
  
  return barProcentage;
}

bool secureProcentage()
{
  byte dominantIngredientPump = getDominantIngredientPump(); //0 -> 7
  for(int i = 0; i<3; i++) //Add or remove up to 3% of dominant ingredient
  {
    if(checkTotalProcentage() < 100)
      pumpProcentages[dominantIngredientPump]++;
    else
      pumpProcentages[dominantIngredientPump]--;
    
    if(checkTotalProcentage() == 100)
      return true;
  }
  return false;
}

byte getDominantIngredientPump()
{
  byte heighestProcentage = 0;
  byte dominantPump = 0;
  for(int i = 0; i<numberOfPumps; i++)
  {
    if(pumpProcentages[i] > heighestProcentage)
      dominantPump = i;
  }
  return dominantPump;
}

byte checkTotalProcentage()
{
  byte procentage = 0;
  for(int i = 0; i<numberOfPumps; i++)
  {
    procentage += pumpProcentages[i];
  }
  return procentage;
}

void drinkLauncherSetup(String drinkName)
{
  tft.fillScreen(0); //Clear screen

  //TOP
  tft.drawFastHLine(0, 30, tftWidth, 0xFFFF);
  tft.drawFastVLine(210, 0, 30, 0xFFFF);
  tft.setTextSize(2);
  tft.setTextColor(0xFFFF);
  tft.setCursor(5, 7);
  tft.print(drinkName);

  tft.drawFastHLine(225, 9, 12, 0xFFFF);
  tft.fillTriangle(225, 5, 225, 13, 220, 9, 0xFFFF);
  tft.fillCircle(230, 20, 4, 0x551F);
  tft.fillCircle(230, 20, 2, 0);
}

short getDrinkStrengthMenu()
{
  short strength = 0;
  currentMenu = STRENGTH_MENU;
  strengthMenuSetup();
  while(!returnFromDrinkLaucher && currentMenu == STRENGTH_MENU)
  {
    if(encoderTurned)
      strength = updateStrength();

    if(encoderPressed)
      currentMenu = DRINK_MENU;
  }
  encoderPressed = false;
  return strength;
}

void strengthMenuSetup()
{
  //clear sizeMenu
  tft.fillRect(0, 31, tftWidth, tftHeight - 31, 0);
  
  //Instrcutions
  tft.setTextSize(3);
  tft.setTextColor(0x06E1);
  tft.setCursor(18, 40);
  tft.print("Choose Drink");
  tft.setCursor(50, 75);
  tft.print("Strenght");

  //Bar Text
  tft.setTextSize(2);
  tft.setTextColor(0xF800);
  tft.setCursor(150, 140);
  tft.print("Strong");
  tft.setTextColor(0xDEFB);
  tft.setCursor(150, 210);
  tft.print("Medium");
  tft.setTextColor(0x07FD);
  tft.setCursor(150, 280);
  tft.print("Soft");

  //Bar Tick Marks
  tft.drawFastHLine(115, 145, 20, 0xFFFF);
  tft.drawFastHLine(115, 180, 20, 0xFFFF);
  tft.drawFastHLine(115, 215, 20, 0xFFFF);
  tft.drawFastHLine(115, 250, 20, 0xFFFF);
  tft.drawFastHLine(115, 285, 20, 0xFFFF);

  showArrow(70, 145, 35, 2, 30, 0x06E1);
  encoderValue = encoderSensitivity*3;
  MaxEncoderValue = encoderSensitivity*5; 
  MinEncoderValue = 1;
  encoderPressed = false;
  encoderTurned = false;
}

short updateStrength()
{
  short strength;
  encoderTurned = false;
  byte newEncoderValue = map(encoderValue, 0, MaxEncoderValue, 1, MaxEncoderValue/encoderSensitivity);

  if (tickPlace == newEncoderValue) //If no change, then no nead to draw again
    return;

  //Clear Previous Arrow
  hideArrow(70, 145, 35, tickPlace-1, 30);

  //Update CurrentPumpSelected
  tickPlace = newEncoderValue;

  switch(tickPlace)
  {
    case 1: 
      strength = 2*strengthTickValue;
      break;
      
    case 2:
      strength = 1*strengthTickValue;
      break;
      
    case 3: 
      strength = 0;
      break;
      
    case 4:
      strength = -1*strengthTickValue;
      break;
      
    case 5:
      strength = -2*strengthTickValue;
      break;
                     
    default:
      break;
  }
  
  //Draw New Arrow
  showArrow(70, 145, 35, tickPlace-1, 30, 0x06E1);
  return strength;
}

unsigned int getDrinkSizeMenu()
{
  unsigned int drinkSize = 0;
  byte currentSelected = 0; 
  drinkSizeMenuSetup();
   
  while(!returnFromDrinkLaucher && currentMenu == SIZE_MENU)
  {
    if(encoderTurned)
      currentSelected = updateSizeSelected(currentSelected);

    if(encoderPressed)
      drinkSize = chooseSelectedSize(currentSelected);
  }

  encoderPressed = false;
  return drinkSize;
}

void drinkSizeMenuSetup()
{
  //Instrcutions
  tft.setTextSize(3);
  tft.setTextColor(0x06E1);
  tft.setCursor(18, 40);
  tft.print("Choose Drink");
  tft.setCursor(90, 75);
  tft.print("SIZE");

  //Options
  tft.setTextSize(2);
  tft.setTextColor(0xDB60);
  tft.setCursor(50, 140);
  tft.print("Size 1");  
  tft.setCursor(50, 180);
  tft.print("Size 2"); 
  tft.setCursor(50, 220);
  tft.print("Size 3");
  if(adminMode)
  {
    tft.setCursor(50, 260);
    tft.print("Custom"); 
  }
  
  
  //Arrow
  showArrow(15, 145, 40, 0, 15, 0x06E1);

  //Add values
  tft.setTextColor(0xFFFF);
  for(int i = 0; i<numberOfDrinkSizes; i++)
  {
    tft.setCursor(150, 140+i*40);
    tft.print(drinkSizes[i]);
    tft.print(" cl");  
  }
  if(adminMode)
  {
    tft.setCursor(150, 260);
    tft.print("0 cl");
  } 
  

  MinEncoderValue = 0;
  encoderValue = MinEncoderValue;
  if(adminMode)
    MaxEncoderValue = encoderSensitivity*(numberOfDrinkSizes+1);
  else
    MaxEncoderValue = encoderSensitivity*numberOfDrinkSizes; 
  encoderPressed = false;
  encoderTurned = false;
}

byte updateSizeSelected(byte currentSelected)
{
  encoderTurned = false;
  byte newEncoderValue = map(encoderValue, 0, MaxEncoderValue, 1, MaxEncoderValue/encoderSensitivity);

  if (currentSelected == newEncoderValue-1) //If no change, then no nead to draw again
    return;

  //Clear Previous Arrow
  hideArrow(15, 145, 40, currentSelected, 15);

  //Update CurrentPumpSelected
  currentSelected = newEncoderValue-1;
 
  //Draw New Arrow
  showArrow(15, 145, 40, currentSelected, 15, 0x06E1);
  return currentSelected;
}

unsigned int chooseSelectedSize(byte currentSelected)
{
  encoderPressed = false;
  
  if(currentSelected > numberOfDrinkSizes-1) //Custom selected
  {
    return customSizeMenu();
  }

  currentMenu = DRINK_MENU; 
  return drinkSizes[currentSelected];
}

unsigned int customSizeMenu()
{
  unsigned int customSize = 0;
  encoderValue = 0;
  MinEncoderValue = 0;
  MaxEncoderValue = 1000; //5 liters max
  
  tft.setTextColor(0x06E1);
  tft.setCursor(50, 260);
  tft.print("Custom");
  
  while(!returnFromDrinkLaucher)
  {
    if(encoderTurned)
      customSize = updateCustomSize(customSize);

    if(encoderPressed)
      break;
  }
  encoderPressed = false;
  currentMenu = DRINK_MENU;
  return customSize;
}

unsigned int updateCustomSize(unsigned int customSize)
{
  encoderTurned = false;
  tft.setTextColor(0);
  tft.setCursor(150, 260);
  tft.print(customSize);
  tft.print(" cl");

  customSize = encoderValue*2;
  
  tft.setTextColor(0xFFFF);
  tft.setCursor(150, 260);
  tft.print(customSize);
  tft.print(" cl");

  return customSize;
}

bool drinkAcceptedMenu(String drinkName, short strength)
{
  tft.fillScreen(0); //Clear screen
  
  //Instrcutions
  tft.setTextSize(3);
  tft.setTextColor(0x06E1);
  tft.setCursor(30, 15);
  tft.print("Drink OK ?");

  //Drink Info
  tft.setTextSize(2);
  tft.setCursor(5, 60);
  tft.setTextColor(0xDB60);
  tft.print("Drink: ");
  tft.setCursor(5, 80);
  tft.setTextColor(0xFFFF);
  tft.print("- ");
  tft.print(drinkName);
  tft.setCursor(5, 110);
  tft.setTextColor(0xDB60);
  tft.print("Amount: ");
  tft.setCursor(5, 130);
  tft.setTextColor(0xFFFF);
  tft.print("- ");
  tft.print(totalDrinkSize);
  tft.print(" centiliter");
  if(strength == strengthTickValue*2)
  {
    tft.setTextColor(0xDB60);
    tft.setCursor(5, 160);
    tft.print("Strengh:");
    tft.setTextColor(0xFFFF);
    tft.setCursor(5, 180);
    tft.print("- Strong");
  }
  else if(strength == strengthTickValue*(-2))
  {
    tft.setTextColor(0xDB60);
    tft.setCursor(5, 160);
    tft.print("Strength:");
    tft.setTextColor(0xFFFF);
    tft.setCursor(5, 180);
    tft.print("- Soft");
  }
  
  //Btn Text
  tft.setCursor(59, 245);
  tft.print("NO");
  tft.setCursor(153, 245);
  tft.print("YES");
  
  //Circles
  tft.fillCircle(70, 280, 15, 0x551F);
  tft.fillCircle(70, 280, 10, 0);
  tft.fillCircle(170, 280, 15, 0xFD60);
  tft.fillCircle(170, 280, 10, 0);

  while(!returnFromDrinkLaucher)
  {
    if(drinkAccepted)
    {
      drinkAccepted = false;
      return true;
    }
  }
  return false;
}
