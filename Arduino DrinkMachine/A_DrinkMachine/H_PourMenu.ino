void pourMenu()
{
  pourMenuSetup();
  while (currentMenu == POUR_MENU)
  {
    if (digitalRead(18)) //buttonPressed
    {
      startPump();
      updateAmountPoured();
    }
    else
      stopPump();

    if(runNextLight)
        nextLight();
  }
  disableStepper();
}

void pourMenuSetup()
{
  //Wait for pump to be enabled
  tft.setTextSize(2);
  tft.fillScreen(0);
  tft.setCursor(40, 160);
  tft.setTextColor(0xFFFF);
  tft.print("Preparing Pump");
  enablePump(currentPumpSelected);
  tft.setCursor(40, 160);
  tft.setTextColor(0);
  tft.print("Preparing Pump");

  //Top
  tft.setTextSize(2);
  tft.setCursor(10, 7);
  tft.setTextColor(0xFFFF);
  tft.print(drinksLoaded[currentPumpSelected - 1]);
  tft.drawFastHLine(0, 30, tftWidth, 0xFFFF);
  tft.drawFastVLine(210, 0, 30, 0xFFFF);

  tft.drawFastHLine(225, 9, 12, 0xFFFF);
  tft.fillTriangle(225, 5, 225, 13, 220, 9, 0xFFFF);
  tft.fillCircle(230, 20, 4, 0x551F);
  tft.fillCircle(230, 20, 2, 0);

  //Instructions
  tft.setTextColor(0x06E1);
  tft.setCursor(8, 65);
  tft.print("   Press & Hold");
  tft.setCursor(8, 85);
  tft.print(" Scroll Button To");

  tft.setTextColor(0xFFFF);
  tft.setTextSize(3);
  tft.setCursor(25, 160);
  tft.print("Pour");
  tft.setCursor(142, 160);
  tft.print("Drink");
  tft.fillRect(115, 150, 10, 50, 0xFBA0);
  tft.fillTriangle(90, 200, 150, 200, 120, 220, 0xFBA0);
  tft.fillRect(90, 221, 60, 6, 0xFBA0);

  //PourAmount
  tft.setTextSize(2);
  tft.fillRect(0, 295, tftWidth, tftHeight - 290, 0xAFF6);
  tft.setTextColor(0xAFF6);
  tft.setCursor(10, 270);
  tft.print("Centiliters Poured");
  tft.setTextColor(0);
  pumpTotalRunTime = 0; //reset amount counter
}

void updateAmountPoured()
{
  uint16_t currentlyPoured = (pumpTotalRunTime + millis() - pumpStartTime) / pumpsCentiliterTime[currentPumpSelected-1];
  
  if (currentlyPoured > amountPoured) //If amount counter should be updated
  {
    tft.setCursor(112, 300); tft.setTextColor(0xAFF6);
    tft.print(amountPoured);
    amountPoured = currentlyPoured;
    tft.setCursor(112, 300); tft.setTextColor(0);
    tft.print(amountPoured);
  }
}
