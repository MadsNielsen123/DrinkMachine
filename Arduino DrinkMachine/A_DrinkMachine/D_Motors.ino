void stepMotor(byte NumberOfSteps)
{
  for(byte i = 0; i<NumberOfSteps; i++)
  {
    digitalWrite(STEPPER_STEP, HIGH);
    delay(stepDelay);
    digitalWrite(STEPPER_STEP, LOW);
    delay(stepDelay);
  }
}

void stepperHome()
{
  
  Serial.println("*Stepper Finding Home*");
  stepperIsHome = false;
  digitalWrite(STEPPER_ENABLE, HIGH);
  delay(50);  
  while(true)
  {
    if(stepperIsHome)
      break;
    else
      stepMotor(1); //Step once
  }
  activePump = 1;
  Serial.println(" - Stepper Home");
}

void enablePump(byte pumpNumber)
{
  if(activePump == 0)
    stepperHome();
   
  Serial.println("*Changing pump*");
  
  if(activePump<pumpNumber)
    stepMotor(25*(pumpNumber-activePump));
  else if(activePump>pumpNumber)
    stepMotor(25*(numberOfPumps-activePump+pumpNumber));

  activePump = pumpNumber;
  Serial.print(" - Pump ");
  Serial.print(activePump);
  Serial.println(" is active");
}

void disableStepper()
{
  digitalWrite(STEPPER_ENABLE, LOW);
  activePump = 0;
}

void startPump(bool dir = 0) //Takes around 1s to startup, can't be canceled before over
{
  if(!pumpRunning)
  {   
    pumpRunning = true;
    digitalWrite(MOTOR_DIR, dir);
    Serial.println("*Beginning acceleration*");
    for(int i = 30; i<=100; i++)
    {
      if(i == 80) //Adjust accordingly
        pumpStartTime = millis();
      analogWrite(MOTOR_PWM, i);
      delay(15);
    }
    Serial.println(" - Max speed");
  }
}

void stopPump()
{
  if(pumpRunning)
  {   
    pumpTotalRunTime += millis() - pumpStartTime; //Add to time previously run  
    pumpRunning = false;
    analogWrite(MOTOR_PWM, 0);
    Serial.println("*Pump Stopped*");
  }
}
