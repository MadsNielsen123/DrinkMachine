void changeLight(char color = 'a') //color ranges from a-t (a = off)
{
  digitalWrite(LED_PREPARE_PIN, HIGH);
  delay(1); //LET IT RECIVE PREPARE SIGNAL
  Serial3.write(color);
  digitalWrite(LED_PREPARE_PIN, LOW);  
}

void nextLight()
{
  runNextLight = false;
  if(lightChar == 19)
    lightChar = 0;
  else
    lightChar++;

  changeLight(char(tolower(alphabet[lightChar])));
}

void hideArrow(byte x, unsigned int y, byte distance, byte increment, byte arrowLength)
{
  tft.drawFastHLine(x, y+increment*distance, arrowLength, 0);
  tft.fillTriangle(x+arrowLength+5, y+increment*distance, x+arrowLength, y+increment*distance+5, x+arrowLength, y+increment*distance-5, 0);
}

void showArrow(byte x, unsigned int y, byte distance, byte increment, byte arrowLength, int color = 0xFFFF)
{
  tft.drawFastHLine(x, y+increment*distance, arrowLength, color);
  tft.fillTriangle(x+arrowLength+5, y+increment*distance, x+arrowLength, y+increment*distance+5, x+arrowLength, y+increment*distance-5, color);
}
