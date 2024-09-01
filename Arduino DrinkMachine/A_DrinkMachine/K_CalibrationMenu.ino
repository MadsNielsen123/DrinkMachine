void calibrationMenu()
{
  tft.setTextSize(2);
  tft.fillScreen(0);
  
  // ------- Wait for pump to be enabled -----
  tft.setTextColor(0xFFFF);
  tft.setCursor(40, 160); 
  tft.print("Preparing Pump");
  
  enablePump(currentPumpSelected);
  
  tft.setTextColor(0);
  tft.setCursor(40, 160);
  tft.print("Preparing Pump");

  //-------- Wait for tube to be filled-------
  tft.setTextColor(0xFFFF);
  tft.setCursor(10, 140);
  tft.print("Press Scoll Button");
  tft.setCursor(20, 160);
  tft.print("When Drink Exits");
  tft.setCursor(60, 180);
  tft.print("The Tube");
  delay(1000);
  encoderPressed = false;
  startPump();
  while(!encoderPressed);
  encoderPressed = false;
  stopPump();
  
  tft.setTextColor(0);
  tft.setCursor(10, 140);
  tft.print("Press Scoll Button");
  tft.setCursor(20, 160);
  tft.print("When Drink Enters");
  tft.setCursor(60, 180);
  tft.print("The Cup...");

  //-------- Calibrate-------
  
  tft.setTextColor(0xFFFF);
  tft.setCursor(15, 120);
  tft.print("Measure 5 cl");
  tft.setCursor(25, 160);
  tft.print("Press encoder to");
  tft.setCursor(30, 180);
  tft.print("Start & finish");
  encoderPressed = false;
  pumpTotalRunTime = 0;
   
  while(!encoderPressed);
  startPump();
  encoderPressed = false;
  while(!encoderPressed);
  stopPump();
  encoderPressed = false;
  disableStepper();
  pumpsCentiliterTime[currentPumpSelected-1] = pumpTotalRunTime/5;
  delay(500);
  currentMenu = PUMP_MENU;
}
