void LEDsOff()
{
  Driver.begin();
  Driver.SetColor(0, 0 , 0);
  Driver.end();
}

void slowCrossFadeLEDs()
{
  while (!endCurrentTask)
  {
    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(i, 0 , 255 - i); //+Red -blue
      Driver.end();
      delay(5);
    }

    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(255 - i, i, 0); //-red +green
      Driver.end();
      delay(5);
    }

    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(0, 255 - i, i); //-Green  +blue
      Driver.end();
      delay(5);
    }
  }
}

void fastCrossFadeLEDs()
{

  while (!endCurrentTask)
  {
    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(i, 0 , 255 - i); //+Red -blue
      Driver.end();
      delay(2);
    }

    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(255 - i, i, 0); //-red +green
      Driver.end();
      delay(2);
    }

    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(0, 255 - i, i); //-Green  +blue
      Driver.end();
      delay(2);
    }
  }
}

void redLEDs()
{
  Driver.begin();
  Driver.SetColor(255, 0, 0);
  Driver.end();
}

void greenLEDs()
{
  Driver.begin();
  Driver.SetColor(0, 255, 0);
  Driver.end();
}

void blueLEDs()
{
  Driver.begin();
  Driver.SetColor(0, 0, 255);
  Driver.end();
}

void slowBlinkRedLEDs()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
  }
}

void slowBlinkGreenLEDs()
{
  while (!endCurrentTask)
  {
    greenLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
  }
}

void slowBlinkBlueLEDs()
{
  while (!endCurrentTask)
  {
    blueLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
  }
}

void fastBlinkRedLEDs()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(300);
    LEDsOff();
    delay(300);
  }
}

void fastBlinkGreenLEDs()
{
  while (!endCurrentTask)
  {
    greenLEDs();
    delay(300);
    LEDsOff();
    delay(300);
  }
}

void fastBlinkBlueLEDs()
{
  while (!endCurrentTask)
  {
    blueLEDs();
    delay(300);
    LEDsOff();
    delay(300);
  }
}

void slowBlinkMultipleLEDs()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
    greenLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
    blueLEDs();
    delay(1000);
    LEDsOff();
    delay(1000);
  }
}

void fastBlinkMultipleLEDs()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(300);
    LEDsOff();
    delay(300);
    greenLEDs();
    delay(300);
    LEDsOff();
    delay(300);
    blueLEDs();
    delay(300);
    LEDsOff();
    delay(300);
  }
}

void slowLEDchange()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(1000);
    greenLEDs();
    delay(1000);
    blueLEDs();
    delay(1000);
  }
}

void fastLEDchange()
{
  while (!endCurrentTask)
  {
    redLEDs();
    delay(300);
    greenLEDs();
    delay(300);
    blueLEDs();
    delay(300);
  }
}

void redPulse()
{
  while (!endCurrentTask)
  {
    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(i, 0, 0);
      Driver.end();
      delay(2);
    }
    for (int i = 255; i > 0 && !endCurrentTask; i--)
    {
      Driver.begin();
      Driver.SetColor(i, 0, 0);
      Driver.end();
      delay(2);
    }
  }
}

void greenPulse()
{
  while (!endCurrentTask)
  {
    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(0, i, 0);
      Driver.end();
      delay(2);
    }
    for (int i = 255; i > 0 && !endCurrentTask; i--)
    {
      Driver.begin();
      Driver.SetColor(0, i, 0);
      Driver.end();
      delay(2);
    }
  }
}

void bluePulse()
{
  while (!endCurrentTask)
  {
    for (int i = 0; i < 255 && !endCurrentTask; i++)
    {
      Driver.begin();
      Driver.SetColor(0, 0, i);
      Driver.end();
      delay(2);
    }
    for (int i = 255; i > 0 && !endCurrentTask; i--)
    {
      Driver.begin();
      Driver.SetColor(0, 0, i);
      Driver.end();
      delay(2);
    }
  }
}

void policeLight()
{
  while (!endCurrentTask)
  {
    while (!endCurrentTask)
  {
    redLEDs();
    delay(300);
    blueLEDs();
    delay(300);
  }
  }
}
