void loop() 
{
  //Go to
  switch (currentMenu)
  {
    case DRINK_MENU:
      drinkMenu();
      break;
      
    case PUMP_MENU:
      pumpMenu(); 
      break;

    case POUR_MENU:
      pourMenu(); 
      break;

    case LOADED_DRINK_MENU:
      loadedDrinkMenu(); 
      break;

    case SIZE_MENU:
      lauchSelectedDrink(); 
      break;

    case CALIBRATION_MENU:
      calibrationMenu();
    
    default:
      break;
  }
}
